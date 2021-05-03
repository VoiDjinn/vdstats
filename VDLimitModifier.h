#ifndef VDLIMITMODIFIER_H
#define VDLIMITMODIFIER_H

#include "VDStat.h"

class VDScLimitModifier : public VDScStatModifier
{
    GDCLASS ( VDScLimitModifier, VDScStatModifier );

protected:
    static void _bind_methods();

    float lower_limit;
    float upper_limit;

    virtual void apply ( Ref<VDScStat> initiator ) override;
    virtual float calculate ( Ref<VDScStat> initiator, float current_value ) const override;

public:
    VDScLimitModifier();

    void set_lower_limit ( float limit );
    float get_lower_limit() const;
    void set_upper_limit ( float limit );
    float get_upper_limit() const;
};

#endif
