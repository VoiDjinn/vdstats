#include "VDStatRelation.h"

VDStatRelation::VDStatRelation(String related_stat_name, float related_value, bool adjusting_base) {
	this->related_stat_name = related_stat_name;
	this->related_value = related_value;
	this->adjusting_base = adjusting_base;
}

void VDStatRelation::_bind_methods(){
	ClassDB::bind_method(D_METHOD("_calculate", "value"), &VDStatRelation::_calculate);

	ClassDB::bind_method(D_METHOD("set_related_stat_name", "stat_name"), &VDStatRelation::set_related_stat_name);
	ClassDB::bind_method(D_METHOD("get_related_stat_name"), &VDStatRelation::get_related_stat_name);
	ClassDB::bind_method(D_METHOD("set_related_value", "value"), &VDStatRelation::set_related_value);
	ClassDB::bind_method(D_METHOD("get_related_value"), &VDStatRelation::get_related_value);
	ClassDB::bind_method(D_METHOD("set_is_adjusting_base", "adjusting_base"), &VDStatRelation::set_is_adjusting_base);
	ClassDB::bind_method(D_METHOD("is_adjusting_base"), &VDStatRelation::is_adjusting_base);
	
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "related_stat_name"), "set_related_stat_name", "get_related_stat_name");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "related_value"), "set_related_value", "get_related_value");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "adjusting_base"), "set_is_adjusting_base", "is_adjusting_base");
}

float VDStatRelation::_calculate(float value){
	return value * related_value;
}

void VDStatRelation::set_related_stat_name(String stat_name){
	this->related_stat_name = stat_name;
}

String VDStatRelation::get_related_stat_name() const {
	return related_stat_name;
}

void VDStatRelation::set_related_value(float value){
	this->related_value = value;
}

float VDStatRelation::get_related_value() const {
	return related_value;
}

void VDStatRelation::set_is_adjusting_base(bool adjusting_base){
	this->adjusting_base = adjusting_base;
}

bool VDStatRelation::is_adjusting_base() const {
	return this->adjusting_base;
}