/**
 * Source file for HVector class.
 *
 * Author: Saravanan Poosanthiram
 */

#include "HVector.h"

#ifdef UNIT_TEST
#include "gtest/gtest.h"
#endif

#include "Vector3d.h"

namespace Core {

HVector::HVector(const Vector3d& v)
{
    this->initialize(v.x(), v.y(), v.z(), 1.0);
}

void HVector::assign(const Vector3d& v)
{
    this->initialize(v.x(), v.y(), v.z(), 1.0);
}

#ifdef UNIT_TEST

TEST(HVectorTest, Simple)
{
}

#endif // UNIT_TEST

} //namespace Core
