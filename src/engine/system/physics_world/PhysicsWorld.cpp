#include "engine/system/physics_world/PhysicsWorld.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadPhysicsWorldBindings();
}

namespace ds
{
bool PhysicsWorld::Initialize(const Config &config)
{
    bool result = true;

    // Send system init message
    ds_msg::SystemInit initMsg;
    initMsg.systemName = "PhysicsWorld";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    return result;
}

void PhysicsWorld::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);
}

void PhysicsWorld::Shutdown()
{
}

void PhysicsWorld::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesReceived, messages);
}

ds_msg::MessageStream PhysicsWorld::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

ScriptBindingSet PhysicsWorld::GetScriptBindings() const
{
    return ds_lua::LoadPhysicsWorldBindings();
}

void PhysicsWorld::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::PhysicsMove:
        {
            ds_msg::PhysicsMove physMove;
            (*messages) >> physMove;

            std::cout << "Entity moved: " << physMove.entity.id
                      << " position: " << physMove.position
                      << " velocity: " << physMove.velocity
                      << " ellipsoidRadius: " << physMove.ellipsoidRadius
                      << std::endl;
            break;
        }
        default:
        {
            messages->Extract(header.size);
            break;
        }
        }
    }
}

void PhysicsWorld::CollideAndSlide(ds_math::Vector3 *position,
                                   ds_math::Vector3 *velocity,
                                   ds_math::Vector3 ellipsoidRadius)
{
    CollisionPacket colPackage;

    // Do collision detection
    // Transform world position and velocity into ellipsoid space
    ds_math::Matrix4 cbm = ds_math::Matrix4::CreateScaleMatrix(
        1.0f / ellipsoidRadius.x, 1.0f / ellipsoidRadius.y,
        1.0f / ellipsoidRadius.z);
    Vector3 ePosition = ds_math::Vector3::Transform(*position, cbm);
    Vector3 eVelocity = ds_math::Vector3::Transform(*velocity, cbm);

    // Build collision package
    colPackage.eRadius = ellipsoidRadius;
    colPackage.r3Position = *position;
    colPackage.r3Velocity = *velocity;
    colPackage.eVelocity = eVelocity;
    colPackage.eNormalizedVelocity = ds_math::Vector3::Normalize(eVelocity);
    colPackage.eBasePoint = ePosition;

    m_collisionRecursionDepth = 0;

    // Final position in ellipsoid space
    ds_math::Vector3 eFinalPosition =
        CollideWithWorld(&colPackage, position, velocity);

    // Convert final position back to r3 space
    ds_math::Matrix4 invCbm = ds_math::Matrix4::CreateScaleMatrix(
        ellipsoidRadius.x, ellipsoidRadius.y, ellipsoidRadius.z);
    ds_math::Vector3 r3FinalPosition =
        ds_math::Vector3::Transform(eFinalPosition, invCbm);

    *position = r3FinalPosition;
}

ds_math::Vector3
PhysicsWorld::CollideWithWorld(PhysicsWorld::CollisionPacket *collisionPacket,
                               ds_math::Vector3 ePosition,
                               ds_math::Vector3 eVelocity)
{
    float unitsPerMeter = 1.0f;
    float unitScale = unitsPerMeter / 100.0f;
    float veryCloseDistance = 0.005f * unitScale;

    // Do we need to keep checking collisions?
    if (m_collisionRecursionDepth > 5)
    {
        return ePosition;
    }

    collisionPacket.eVelocity = eVelocity;
    collisionPacket.eNormalizedVelocity =
        ds_math::Vector3::Normalize(eVelocity);
    collisionPacket.eBasePoint = ePosition;
    collisionPacket.foundCollision = false;

    MoveWithCollisions(&collisionPacket);

    // If no collision we just move along the velocity
    if (collisionPacket.foundCollision == false)
    {
        return ePosition + eVelocity;
    }

    // Else, collision occured
    // Original destination point:
    ds_math::Vector3 eDestinationPoint = ePosition + eVelocity;
    ds_math::Vector3 eNewBasePoint = ePosition;

    /*
     * Only update if we are not already very close and if so only move close to
     * the intersection point, not to it. We don't want to get embedded in the
     * triangle.
     */
    if (collisionPacket.eNearestDistance >= veryCloseDistance)
    {
        ds_math::Vector3 v = eVelocity;
        v.Normalize();
        v = v * (float)(collisionPacket.eNearestDistance - veryCloseDistance);
        eNewBasePoint = collisionPacket.eBasePoint + v;

        /*
         * Adjust polygon intersection point (so that sliding plane will be
         * unaffected by the fact that we move slightly less than the collision
         * tells us).
         */
        v.Normalize();
        collisionPacket.eIntersectionPoint -= veryCloseDistance * v;
    }

    // Determine the sliding plane
    ds_math::Vector3 slidePlaneOrigin = collisionPacket.eIntersectionPoint;
    ds_math::Vector3 slidePlaneNormal =
        eNewBasePoint - collisionPacket.eIntersectionPoint;
    slidePlaneNormal.Normalize();
    ds_math::Plane slidingPlane = ds_math::Plane(
        slidePlaneNormal,
        -ds_math::Vector3::Dot(slidePlaneOrigin, slidePlaneNormal));

    ds_math::Vector3 eNewDestinationPoint =
        eDestinationPoint -
        SignedDistanceFromPlaneToPoint(slidingPlane, eDestinationPoint) *
            slidePlaneNormal;

    // Generate the slide vector, which will become our new velocity vector for
    // the next iteration
    ds_math::Vector3 eNewVelocityVector =
        eNewDestinationPoint - collisionPacket.eIntersectionPoint;

    // Recurse
    // Don't recurse if new velocity is very small
    // NOTE: may need to remove this, look at what happens with very small
    // initial velocity vectors and very close positions.
    if (eNewVelocityVector.Length() < veryCloseDistance)
    {
        return eNewBasePoint;
    }

    ++collisionRecursionDepth;

    return CollideWithWorld(collisionPacket);
}

void PhysicsWorld::MoveWithCollisions(CollisionPacket *colPackage)
{
    if (colPackage->eVelocity != ds_math::Vector3(0.0f, 0.0f, 0.0f))
    {
        for (int i = 0; i < trianglesWorld.size(); ++i)
        {
            ds_math::Matrix4 cbm = ds_math::Matrix4::CreateScaleMatrix(
                (1.0f / colPackage->eRadius.x), (1.0f / colPackage->eRadius.y),
                (1.0f / colPackage->eRadius.z));

            // Points in ellipsoid space
            ds_math::Vector3 ep0 =
                ds_math::Vector3::Transform(trianglesWorld[i].p0, cbm);
            ds_math::Vector3 ep1 =
                ds_math::Vector3::Transform(trianglesWorld[i].p1, cbm);
            ds_math::Vector3 ep2 =
                ds_math::Vector3::Transform(trianglesWorld[i].p2, cbm);

            CheckTriangle(colPackage, ep0, ep1, ep2);
        }
    }
}

void PhysicsWorld::CheckTriangle(CollisionPacket *colPackage,
                                 const ds_math::Vector3 &p0,
                                 const ds_math::Vector3 &p1,
                                 const ds_math::Vector3 &p2)
{
    ds_math::Plane trianglePlane = ds_math::Plane(p2, p1, p0);

    // Only check front faces of triangles
    if (IsFrontFacingTo(trianglePlane, colPackage->eNormalizedVelocity))
    {
        double t0, t1;
        bool embeddedInPlane = false;

        double signedDistancetoTrianglePlane = SignedDistanceFromPlaneToPoint(
            trianglePlane, colPackage->eBasePoint);
        float normalDotVelocity =
            ds_math::Vector3::Dot(trianglePlane.Normal(), colPackage.eVelocity);

        // If Sphere is travelling parallel to palne (normal dot velocity
        // approximately equal to 0)
        if (normalDotVelocity > -10e-7f && normalDotVelocity < 10e-7f)
        {
            if (fabs(signedDistanceToTrinaglePlane) >= 1.0f)
            {
                // Sphere is not embedded in plane, no collision possible
                return;
            }
            else
            {
                // Sphere is embedded in plane and is travelling parallel to
                // plane therefore intersects in whole range [0 .. 1]
                embeddedInPlane = true;
                t0 = 0.0;
                t1 = 1.0;
            }
        }
        // Sphere not travelling parallel to plane
        else
        {
            // Calculate intersection interval
            t0 = (-1.0f - signedDistanceToTrianglePlane) / normalDotVelocity;
            t1 = (1.0f - signedDistanceToTrianglePlane) / normalDotVelocity;

            // Make sure t0 < t1
            if (t0 > t1)
            {
                double temp = t1;
                t1 = t0;
                t0 = temp;
            }

            /*
             * If both t0 and t1 are outside [0, 1] range.
             *
             * Note: The below works because we know that t0 <= t1 from above.
             * Therefore if t0 is greater than 1.0f, t1 must be equal to or
             * greater than 1.0f and therefore they are both outside range. If
             * t1 is < 0.0f the t0 must be <= 0.0f and therefore they are both
             * outside range.
             */
            if (t0 > 1.0f || t1 < 0.0f)
            {
                // No collision possible
                return;
            }

            // Clamp to [0, 1]
            if (t0 < 0.0)
            {
                t0 = 0.0;
            }
            if (t1 < 0.0)
            {
                t1 = 0.0;
            }
            if (t0 > 1.0)
            {
                t0 = 1.0;
            }
            if (t1 > 1.0)
            {
                t1 = 1.0;
            }
        }

        /*
         * We now have two time values: t0 and t1, between which the swept
         * sphere intersects with the triangle plane. If any collision is to
         * occur, it must happen within this interval.
         */
        ds_math::Vector3 collisionPoint = ds_math::Vector(0.0f, 0.0f, 0.0f);
        bool foundCollision = false;
        float t = 1.0f;

        /*
         * Check for collision inside the triangle. If this happens it must be
         * at time t0 as this is when the sphere rests on the front side of the
         * triangle plane. This can only happen if the sphere is not embedded in
         * the triangle plane, an embedded sphere can only collide against a
         * vertex or edge of a triangle.
         */
        if (embeddedInPlane == false)
        {
            ds_math::Vector3 planeIntersectionPoint =
                (colPackage->eBasePoint - trianglePlane.Normal()) +
                (colPackage->eVolocity * (float)t0);

            if (CheckPointInTrianglePlane(p0, p1, p2, planeIntersectionPoint))
            {
                foundCollision = true;
                t = (float)t0;
                collisionPacket = planeIntersectionPoint;
            }
        }

        /*
         * If we haven't found a collision already, check swept sphere against
         * points and edges of triangles.
         */
        if (foundCollision == false)
        {
            ds_math::Vector3 eVelocity = colPackage->eVelocity;
            ds_math::Vector3 eBasePoint = colPackage->eBasePoint;
            float velocitySquaredLength =
                ds_math::Vector3::Dot(eVelocity, eVelocity);
            float a, b, c; // Parameters for quadratic equation
            float newT = 0.0f;

            /*
             * For each vertex or edge a quadratic equation has to be solved. We
             * parameterize the equation as a*t^2 + b*t + c = 0 and below we
             * calculate the parameters a, b and c for each test.
             */

            // Check against the points:
            a = velocitySquaredLength;

            // P0
            b = 2.0f * (ds_math::Vector3::Dot(eVelocity, eBasePoint - p0));
            c = ds_math::Vector3::Dot((p0 - eBasePoint), (p0 - eBasePoint)) -
                1.0f;
            if (GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollision = true;
                collisionPoint = p0;
            }

            // P1
            b = 2.0f * (ds_math::Vector3::Dot(eVelocity, eBasePoint - p1));
            c = ds_math::Vector3((p1 - eBasePoint), (p1 - eBasePoint)) - 1.0f;
            if (GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollision = true;
                collisionPoint = p1;
            }

            // P2
            b = 2.0f * (ds_math::Vector3::Dot(eVelocity, eBasePoint - p2));
            c = ds_math::Vector3::Dot((p2 - eBasePoint), (p2 - eBasePoint)) -
                1.0f;
            if (GetLowestRoot(a, b, c, t, &newT))
            {
                t = newT;
                foundCollision = true;
                collisionPoint = p2;
            }

            /* 
             * Check against edges, note that we have to check against
             * the edges even if we found a collision at one of the
             * points because we need to see if the swept sphere
             * collides with one of the edges at a smaller time
             * value.
             */
            // p0 -> p1
        }
    }
}
}
