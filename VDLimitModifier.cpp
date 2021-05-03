#include "VDLimitModifier.h"

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
