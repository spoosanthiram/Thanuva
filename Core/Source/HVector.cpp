/**
* Author: Saravanan Poosanthiram
* $LastChangedBy: ps $
* $LastChangedDate: 2015-03-14 12:57:44 -0400 (Sat, 14 Mar 2015) $
*/

#include "HVector.h"

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

} //namespace Core
