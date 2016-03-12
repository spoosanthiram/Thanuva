/*
 * Model: Model objects for Thanuva
 *
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef MODEL_BOX_H
#define MODEL_BOX_H

#include <AlgoBase.h>

#include "ModelObject.h"

namespace Model {

class Box : public ModelObject
{
public:
    struct Limiter
    {
        Limiter()
            : xlow{-1.0}
            , xhigh{1.0}
            , ylow{-1.0}
            , yhigh{1.0}
            , zlow{-1.0}
            , zhigh{1.0}
        {}
        Limiter(double xl, double xh, double yl, double yh, double zl, double zh)
            : xlow{xl}
            , xhigh{xh}
            , ylow{yl}
            , yhigh{yh}
            , zlow{zl}
            , zhigh{zh}
        {}

        bool isValid() const
        {
            return (xlow >= xhigh || ylow >= yhigh || zlow >= zhigh) ? false : true;
        }

        bool operator==(const Limiter& rhs) const
        {
            return psa::isequal(rhs.xlow, xlow) && psa::isequal(rhs.xhigh, xhigh)
                && psa::isequal(rhs.ylow, ylow) && psa::isequal(rhs.yhigh, yhigh)
                && psa::isequal(rhs.zlow, zlow) && psa::isequal(rhs.zhigh, zhigh);
        }
        bool operator!=(const Limiter& rhs) const { return !(rhs == *this); }

        double xlow, xhigh;
        double ylow, yhigh;
        double zlow, zhigh;
    };

public:
    Box(const Project& project) : ModelObject{project} {}
    Box(const Project& project, const Limiter& limiter);
    Box(const Box& rhs) = delete; // TODO: needs to be implemented

    Box& operator=(const Box& rhs) = delete; // TODO: needs to be implemented

    Type type() const override { return Type::Box; }
    const Limiter& limiter() const { return m_limiter; }

    void setLimiter(const Limiter& limiter, Core::EmitSignal emitSignal = Core::EmitSignal::Emit);

protected:
    void loadModel(const boost::property_tree::ptree& modelPropTree) override;
    void saveModel(boost::property_tree::ptree& modelPropTree) override;

private:
    Limiter m_limiter{};
};

} // namespace Model

#endif // MODEL_BOX_H
