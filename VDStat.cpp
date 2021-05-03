#include "VDStat.h"
#include "core/os/os.h"


// ************************
// ******** VDScStat ********
// ************************


VDScStat::VDScStat(){
	this->name = "";
	this->base_value = 0;
	this->modificated_value = 0;
}

void VDScStat::_bind_methods(){
	ClassDB::bind_method(D_METHOD("apply_modifier", "modifier", "modifier_name"), &VDScStat::apply_modifier);
	ClassDB::bind_method(D_METHOD("discard_modifier", "modifier_name", "recalculate_after"), &VDScStat::discard_modifier, DEFVAL(true));

	ClassDB::bind_method(D_METHOD("set_stat_name", "name"), &VDScStat::set_stat_name);
	ClassDB::bind_method(D_METHOD("get_stat_name"), &VDScStat::get_stat_name);
	ClassDB::bind_method(D_METHOD("get_modifier", "modifier"), &VDScStat::get_modifier);
	ClassDB::bind_method(D_METHOD("get_modifier_value", "modifier"), &VDScStat::get_modifier_value);
	ClassDB::bind_method(D_METHOD("get_modificated_value"), &VDScStat::get_modificated_value);
	ClassDB::bind_method(D_METHOD("set_base_value", "value"), &VDScStat::set_base_value);
	ClassDB::bind_method(D_METHOD("get_base_value"), &VDScStat::get_base_value);

	ClassDB::bind_method(D_METHOD("handle_modifier_value_changed"), &VDScStat::handle_modifier_value_changed);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_stat_name", "get_stat_name");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "base_value"), "set_base_value", "get_base_value");
	ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::REAL, "new_value"), PropertyInfo(Variant::REAL, "old_value")));
}

void VDScStat::apply_modifier(Ref<VDScStatModifier> modifier, String modifier_name){
	if(!modifiers.has(modifier_name)){
		modifiers.set(modifier_name, modifier);
		// modifier->connect(VDScStatModifier::NAME_SIGNAL_VALUE_CHANGED, this, VDScStat::NAME_METHOD_HANDLER_RECALCULATE, arguments);
		modifier->connect("value_changed", this, "handle_modifier_value_changed");
		if(modifier->is_final_modifier()) modifiers_finals.push_back(modifier);
		else modifiers_regulars.push_back(modifier);
		modifier->apply_modifier(this, modifier_name);
	}
	else {
		Ref<VDScStatModifier> old_modifier = get_modifier(modifier_name);
		if(old_modifier != modifier) {
			discard_modifier(modifier_name, false);
			apply_modifier(modifier, modifier_name);
		}
	}
}

Ref<VDScStatModifier> VDScStat::discard_modifier(String modifier_name, bool recalculate_after) {
	if(modifiers.has(modifier_name)){
		Ref<VDScStatModifier> modifier = modifiers.get(modifier_name);
		if(modifier->is_final_modifier()) modifiers_finals.erase(modifier);
		else modifiers_regulars.erase(modifier);
		modifiers.erase(modifier_name);
		modifier->discard_modifier(this, modifier_name);
		// modifier->disconnect(VDScStatModifier::NAME_SIGNAL_VALUE_CHANGED, this,  VDScStat::NAME_METHOD_HANDLER_RECALCULATE);
		modifier->disconnect("value_changed", this,  "handle_modifier_value_changed");
		if(recalculate_after) recalculate();
		return modifier;
	}
	return nullptr;
}

float VDScStat::recalculate(){
	float old_modificated_value = this->modificated_value;
	float new_modificated_value = this->base_value;
	for(int i = 0; i < this->modifiers_regulars.size(); i++){
		Ref<VDScStatModifier> regular_modifier = this->modifiers_regulars[i];
		new_modificated_value = regular_modifier->calculate_modifier(this, new_modificated_value);
	}
	for(int i = 0; i < this->modifiers_finals.size(); i++){
		Ref<VDScStatModifier> final_modifier = this->modifiers_finals[i];
		new_modificated_value = final_modifier->calculate_modifier(this, new_modificated_value);
	}
	if(new_modificated_value != old_modificated_value){
		this->modificated_value = new_modificated_value;
		emit_signal("value_changed", new_modificated_value, old_modificated_value);
		if(this->owner != nullptr) this->owner->notify_stat_change(this, new_modificated_value, old_modificated_value);
	}
	return new_modificated_value;
}

void VDScStat::handle_modifier_value_changed(float new_value, float old_value, Ref<VDScStatModifier> modifier){
	// TODO: maybe later try to make it more efficient,
	// for now the easiest way
	recalculate();
}

void VDScStat::set_stat_name(String name){
	ERR_FAIL_COND_MSG(this->get_owner().is_valid(), "Stat name cannot be changed if it is owned by a stat component.")
	this->name = name;
}

String VDScStat::get_stat_name() const {
	return this->name;
}

Ref<VDScStats> VDScStat::get_owner() const {
	return this->owner;
}

void VDScStat::set_base_value(float value){
	if(value != this->base_value){
		this->base_value = value;
		recalculate();
	}
}

float VDScStat::get_base_value() const {
	return this->base_value;
}

Ref<VDScStatModifier> VDScStat::get_modifier(String modifier) const {
	if(this->modifiers.has(modifier)){
		Ref<VDScStatModifier> mod = modifiers[modifier];
		return mod;
	}
	return nullptr;
}

float VDScStat::get_modifier_value(String modifier) const {
	Ref<VDScStatModifier> mod = this->get_modifier(modifier);
	if(!mod.is_valid()){
		return mod->get_calculated_value();
	}
	return 0;
}

float VDScStat::get_modificated_value() const {
	return this->modificated_value;
}

// ********************************
// ******** VDScStatModifier ********
// ********************************

VDScStatModifier::VDScStatModifier(){
	this->final_modifier = false;
}

void VDScStatModifier::_bind_methods(){
	ClassDB::bind_method(D_METHOD("get_applied_key"), &VDScStatModifier::get_applied_key);
	ClassDB::bind_method(D_METHOD("set_is_final_modifier", "is_final"), &VDScStatModifier::set_is_final_modifier);
	ClassDB::bind_method(D_METHOD("is_final_modifier"), &VDScStatModifier::is_final_modifier);
	ClassDB::bind_method(D_METHOD("get_calculated_value"), &VDScStatModifier::get_calculated_value);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "final_modifier"), "set_is_final_modifier", "is_final_modifier");

	ADD_SIGNAL(MethodInfo("value_changed", PropertyInfo(Variant::REAL, "new_value"), PropertyInfo(Variant::REAL, "old_value"), PropertyInfo(Variant::OBJECT, "modifier")));
}

void VDScStatModifier::apply(Ref<VDScStat> initiator){}
void VDScStatModifier::discard(Ref<VDScStat> initiator){}
float VDScStatModifier::calculate(Ref<VDScStat> initiator, float current_value) const { return 0.0;}

void VDScStatModifier::apply_modifier(Ref<VDScStat> initiator, String key){
	this->modifier_key = key;
	apply(initiator);
}

void VDScStatModifier::discard_modifier(Ref<VDScStat> initiator, String key){
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

// *************************
// ******** VDScStats ********
// *************************


VDScStats::VDScStats() {}

// TODO: implement Limiters for stats (Min/Max cannot be exceeded)
// TODO: more accessible methods and functionality
// TODO: modificators removal functionality

void VDScStats::_bind_methods(){
	ClassDB::bind_method(D_METHOD("add_stat", "stat"), &VDScStats::add_stat);
	ClassDB::bind_method(D_METHOD("get_stat", "attribute_name"), &VDScStats::get_stat);
	ClassDB::bind_method(D_METHOD("remove_stat", "attribute_name"), &VDScStats::remove_stat);
	ClassDB::bind_method(D_METHOD("get_stats"), &VDScStats::get_stats);

	ADD_SIGNAL(MethodInfo("stat_changed", PropertyInfo(Variant::OBJECT, "stat"), PropertyInfo(Variant::REAL, "new_value"), PropertyInfo(Variant::REAL, "old_value")));
	ADD_SIGNAL(MethodInfo("stat_added", PropertyInfo(Variant::OBJECT, "stat")));
	ADD_SIGNAL(MethodInfo("stat_removed", PropertyInfo(Variant::OBJECT, "stat")));
}

void VDScStats::notify_stat_change(Ref<VDScStat> stat, float new_value, float old_value){
	emit_signal("stat_changed", stat, new_value, old_value);
}

void VDScStats::finalize(){}

void VDScStats::add_stat(Ref<VDScStat> stat){
	String attribute_name = stat != nullptr ? stat->get_stat_name() : "";
	if(!stats.has(attribute_name)) {
		stat->owner = Ref<VDScStats>(this);
		stats.set(attribute_name, stat);
		emit_signal("stat_added", stat);
	}
}

Ref<VDScStat> VDScStats::get_stat(String attribute_name) const {
	if(stats.has(attribute_name)){
		Ref<VDScStat> gotten_stat = stats[attribute_name];
		return gotten_stat;
	}
	return nullptr;
}

bool VDScStats::has_stat(String attribute_name) const {
	return stats.has(attribute_name);
}

Ref<VDScStat> VDScStats::remove_stat(String attribute_name) {
	if(stats.has(attribute_name)){
		Ref<VDScStat> removed_stat = stats[attribute_name];
		removed_stat->owner = Ref<VDScStats>(nullptr);
		stats.erase(attribute_name);
		emit_signal("stat_removed", removed_stat);
		return removed_stat;
	}
	return nullptr;
}

Array VDScStats::get_stats(){
	List<String> keys;
	this->stats.get_key_list(&keys);
	Array returnedStats;
	for(int i = 0; i < keys.size(); i++){
		String stat_key = keys[i];
		Ref<VDScStat> retStat = this->stats[stat_key];
		returnedStats.append(retStat);
	}
	return returnedStats;
}

