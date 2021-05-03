#include "VDStatDependentModifier.h"

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
