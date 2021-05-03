#ifndef VDFLATMODIFIER_H
#define VDFLATMODIFIER_H

#include "VDStat.h"

class VDScFlatModifier : public VDScStatModifier
{
    GDCLASS ( VDScFlatModifier, VDScStatModifier );

    float flat_value;

protected:
    static void _bind_methods();

    virtual void apply ( Ref<VDScStat> initiator ) override;
    virtual float calculate ( Ref<VDScStat> initiator, float current_value ) const override;

public:
    VDScFlatModifier();

    void set_flat_value ( float value );
    float get_flat_value() const;
};

#endif
