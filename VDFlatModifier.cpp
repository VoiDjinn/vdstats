#include "VDFlatModifier.h"

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
