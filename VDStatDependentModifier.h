#ifndef VDSTATDEPENDENTMODIFIER_H
#define VDSTATDEPENDENTMODIFIER_H

#include "VDStat.h"

class VDScStatDependentModifier : public VDScStatModifier
{
    GDCLASS ( VDScStatDependentModifier, VDScStatModifier );

    friend class VDScStat;

    Ref<VDScStat> dependent_stat;
    float factor = 1.0;

    void handle_dependent_value_change ( float new_value, float old_value );

protected:
    static void _bind_methods();

    virtual void apply ( Ref<VDScStat> initiator ) override ;
    virtual void discard ( Ref<VDScStat> initiator ) override;
    virtual float calculate ( Ref<VDScStat> initiator, float current_value ) const override;
public:
    VDScStatDependentModifier();

    void set_dependent_stat ( Ref<VDScStat> stat );
    Ref<VDScStat> get_dependent_stat() const;

    void set_factor ( float factor );
    float get_factor() const;
};

#endif
