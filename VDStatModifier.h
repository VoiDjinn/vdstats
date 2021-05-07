#ifndef VDSTATMODIFIER_H
#define VDSTATMODIFIER_H

#include "core/reference.h"

class VDScStat;
/////
// VDScStatModifier
/////
class VDScStatModifier : public Reference {
    GDCLASS(VDScStatModifier, Reference);

    friend class VDScStat;

    String modifier_key = "";
    bool final_modifier = false;

    void apply_modifier(Ref<VDScStat> initiator, String key);
    void discard_modifier(Ref<VDScStat> initiator, String key);
    float calculate_modifier(Ref<VDScStat> initiator, float current_value);

protected:
    float calculated_value = 0;

    static void _bind_methods();
    virtual void apply(Ref<VDScStat> initiator);
    virtual void discard(Ref<VDScStat> initiator);
    virtual float calculate(Ref<VDScStat> initiator, float current_value) const;
public:
    VDScStatModifier();

    String get_applied_key() const;
    void set_is_final_modifier(bool is_final);
    bool is_final_modifier() const;
    float get_calculated_value() const;
};
//////////
///// Default modifiers
//////////
/////
// VDScFlatModifier
/////
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
/////
// VDScLimitModifier
/////
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
/////
// VDScStatDependentModifier
/////
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
