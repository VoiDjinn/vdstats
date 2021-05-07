#include "VDStatModifier.h"
#include "VDStat.h"

// ********************************
// ******** VDScStatModifier ********
// ********************************
VDScStatModifier::VDScStatModifier() {
    this->final_modifier = false;
}

void VDScStatModifier::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_applied_key"), &VDScStatModifier::get_applied_key);
    ClassDB::bind_method(D_METHOD("set_is_final_modifier", "is_final"), &VDScStatModifier::set_is_final_modifier);
    ClassDB::bind_method(D_METHOD("is_final_modifier"), &VDScStatModifier::is_final_modifier);
    ClassDB::bind_method(D_METHOD("get_calculated_value"), &VDScStatModifier::get_calculated_value);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "final_modifier"), "set_is_final_modifier", "is_final_modifier");

    ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::REAL, "new_value"), PropertyInfo(Variant::REAL, "old_value"), PropertyInfo(Variant::OBJECT, "modifier")));
}

void VDScStatModifier::apply(Ref<VDScStat> initiator) {}
void VDScStatModifier::discard(Ref<VDScStat> initiator) {}
float VDScStatModifier::calculate(Ref<VDScStat> initiator, float current_value) const {
    return 0.0;
}

void VDScStatModifier::apply_modifier(Ref<VDScStat> initiator, String key) {
    this->modifier_key = key;
    apply(initiator);
}

void VDScStatModifier::discard_modifier(Ref<VDScStat> initiator, String key) {
    this->modifier_key = "";
    discard(initiator);
}

float VDScStatModifier::calculate_modifier(Ref<VDScStat> initiator, float current_value) {
    float new_calculated_value = current_value;
    new_calculated_value = calculate(initiator, current_value);
    return new_calculated_value;
}

String VDScStatModifier::get_applied_key() const {
    return this->modifier_key;
}

void VDScStatModifier::set_is_final_modifier(bool is_final) {
    ERR_FAIL_COND_MSG(this->modifier_key.length() > 0, "Final flag cannot be changed as long as the modifer is applied to a stat.")
    this->final_modifier = is_final;
}

bool VDScStatModifier::is_final_modifier() const {
    return this->final_modifier;
}

float VDScStatModifier::get_calculated_value() const {
    return this->calculated_value;
}
//////////
///// Default modifiers
//////////
/////
// VDScFlatModifier
/////
VDScFlatModifier::VDScFlatModifier() : VDScStatModifier() {
    this->flat_value = 0;
}

void VDScFlatModifier::_bind_methods() {
    ClassDB::bind_method ( D_METHOD ( "set_flat_value", "value" ), &VDScFlatModifier::set_flat_value );
    ClassDB::bind_method ( D_METHOD ( "get_flat_value" ), &VDScFlatModifier::get_flat_value );

    ADD_PROPERTY ( PropertyInfo ( Variant::REAL, "flat_value" ), "set_flat_value", "get_flat_value" );
}

void VDScFlatModifier::apply ( Ref<VDScStat> initiator ) {
    float old_value = this->calculated_value;
    this->calculated_value = this->flat_value;
    emit_signal ( "value_changed", this->calculated_value, old_value, this );
}

float VDScFlatModifier::calculate ( Ref<VDScStat> initiator, float current_value ) const {
    float new_calculated_value = current_value + this->flat_value;
    return new_calculated_value;
}

void VDScFlatModifier::set_flat_value ( float value ) {
    if ( value != this->flat_value ) {
        float old_value = this->flat_value;
        this->flat_value = value;
        this->calculated_value = this->flat_value;
        emit_signal ( "value_changed", this->calculated_value, old_value, this );
    }
}

float VDScFlatModifier::get_flat_value() const {
    return this->flat_value;
}
/////
// VDScLimitModifier
/////
VDScLimitModifier::VDScLimitModifier() : VDScStatModifier() {
    this->lower_limit = 0;
    this->upper_limit = 0;
}

void VDScLimitModifier::_bind_methods() {
    ClassDB::bind_method ( D_METHOD ( "set_lower_limit", "limit" ), &VDScLimitModifier::set_lower_limit );
    ClassDB::bind_method ( D_METHOD ( "get_lower_limit" ), &VDScLimitModifier::get_lower_limit );
    ClassDB::bind_method ( D_METHOD ( "set_upper_limit", "limit" ), &VDScLimitModifier::set_upper_limit );
    ClassDB::bind_method ( D_METHOD ( "get_upper_limit" ), &VDScLimitModifier::get_upper_limit );
}

void VDScLimitModifier::apply ( Ref<VDScStat> initiator ) {
    float initiated_value = initiator->get_modificated_value();
    float check_limited = calculate ( initiator, initiated_value );
    if ( initiated_value != check_limited ) {
        emit_signal ( "value_changed", check_limited, initiated_value );
    }
}

float VDScLimitModifier::calculate ( Ref<VDScStat> initiator, float current_value ) const {
    float limited_value = current_value;
    if ( limited_value > this->upper_limit ) {
        limited_value = this->upper_limit;
    } else if ( limited_value < this->lower_limit ) {
        limited_value = this->lower_limit;
    }
    return limited_value;
}

// TODO: emit signal for recalculation with new base values
void VDScLimitModifier::set_lower_limit ( float limit ) {
    this->lower_limit = limit;
}

float VDScLimitModifier::get_lower_limit() const {
    return this->lower_limit;
}

void VDScLimitModifier::set_upper_limit ( float limit ) {
    this->upper_limit = limit;
}

float VDScLimitModifier::get_upper_limit() const {
    return this->upper_limit;
}
/////
// VDScStatDependentModifier
/////
VDScStatDependentModifier::VDScStatDependentModifier() : VDScStatModifier() {}

void VDScStatDependentModifier::_bind_methods() {
    ClassDB::bind_method ( D_METHOD ( "set_dependent_stat", "stat" ), &VDScStatDependentModifier::set_dependent_stat );
    ClassDB::bind_method ( D_METHOD ( "get_dependent_stat" ), &VDScStatDependentModifier::get_dependent_stat );
    ClassDB::bind_method ( D_METHOD ( "set_factor", "factor" ), &VDScStatDependentModifier::set_factor );
    ClassDB::bind_method ( D_METHOD ( "get_factor" ), &VDScStatDependentModifier::get_factor );

    ADD_PROPERTY ( PropertyInfo ( Variant::OBJECT, "dependent_stat" ), "set_dependent_stat", "get_dependent_stat" );
    ADD_PROPERTY ( PropertyInfo ( Variant::REAL, "factor" ), "set_factor", "get_factor" );
}

void VDScStatDependentModifier::handle_dependent_value_change ( float new_value, float old_value ) {
    if ( this->calculated_value != new_value ) {
        float old_calculated_value = this->calculated_value;
        this->calculated_value = new_value * this->factor;
        emit_signal ( "value_changed", calculated_value, old_calculated_value, this );
    }
}

void VDScStatDependentModifier::apply ( Ref<VDScStat> initiator ) {
    if ( this->dependent_stat.is_valid() ) {
        this->dependent_stat->connect ( "value_changed", this, "handle_dependent_value_change" );
    }
}

void VDScStatDependentModifier::discard ( Ref<VDScStat> initiator ) {
    if ( this->dependent_stat.is_valid() ) {
        this->dependent_stat->disconnect ( "value_changed", this, "handle_dependent_value_change" );
    }
}

float VDScStatDependentModifier::calculate ( Ref<VDScStat> initiator, float current_value ) const {
    return this->calculated_value;
}

// TODO: revamp logic of connecting with new set stat + disconnect possible old dependent stat
void VDScStatDependentModifier::set_dependent_stat ( Ref<VDScStat> stat ) {
    this->dependent_stat = stat;
}

Ref<VDScStat> VDScStatDependentModifier::get_dependent_stat() const {
    return this->dependent_stat;
}

void VDScStatDependentModifier::set_factor ( float factor ) {
    this->factor = factor;
}

float VDScStatDependentModifier::get_factor() const {
    return this->factor;
}
