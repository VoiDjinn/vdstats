#include "VDStatsTemplate.h"

VDStatsTemplate::VDStatsTemplate() {}

void VDStatsTemplate::_bind_methods(){
	ClassDB::bind_method(D_METHOD("init_by_entry", "entry"), &VDStatsTemplate::init_by_entry);
	
	ClassDB::bind_method(D_METHOD("set_stat_templates", "templates"), &VDStatsTemplate::set_stat_templates);
	ClassDB::bind_method(D_METHOD("get_stat_templates"), &VDStatsTemplate::get_stat_templates);
	
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "stat_templates"), "set_stat_templates", "get_stat_templates");
}

void VDStatsTemplate::init_by_entry(Dictionary entry){
	// #	name = entry["name"]
	// #	for mod in entry["attr_mod"]:
	// #		stat_templates[mod] = entry["attr_mod"].get(mod)
	Array stat_names = entry.keys();
	for(int i = 0; i < stat_names.size(); i++){
		String stat_name = stat_names[i];
		stat_templates[stat_name] = entry[stat_name];
	}
}

void VDStatsTemplate::set_stat_templates(Dictionary templates){
	this->stat_templates = templates;
}

Dictionary VDStatsTemplate::get_stat_templates() const {
	return this->stat_templates;
}