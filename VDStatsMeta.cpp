#include "VDStatsMeta.h"

VDStatsMeta::VDStatsMeta() {}

void VDStatsMeta::_bind_methods(){
	ClassDB::bind_method(D_METHOD("add_relation", "source_attribute", "target_attribute", "value", "adjusting_base"), &VDStatsMeta::add_relation);
	ClassDB::bind_method(D_METHOD("calculate_relations", "source_attribute", "value"), &VDStatsMeta::calculate_relations);

	ClassDB::bind_method(D_METHOD("set_stat_relations", "relations"), &VDStatsMeta::set_stat_relations);
	ClassDB::bind_method(D_METHOD("get_stat_relations"), &VDStatsMeta::get_stat_relations);
	
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "stat_relations"), "set_stat_relations", "get_stat_relations");
}

void VDStatsMeta::add_relation(String source_attribute, String target_attribute, float value, bool adjusting_base){
	Array lst_rels;
	if(stat_relations.has(source_attribute)){
		lst_rels = stat_relations[source_attribute];
	}
	Ref<VDStatRelation> nrelation = Ref<VDStatRelation>(memnew(VDStatRelation(target_attribute, value, adjusting_base)));
	lst_rels.append(nrelation);
	stat_relations[source_attribute] = lst_rels;
}

Dictionary VDStatsMeta::calculate_relations(String source_attribute, float value) const {
	Dictionary new_vals;
	if(stat_relations.has(source_attribute)){
		Array rels = stat_relations[source_attribute];
		for(int i = 0; i < rels.size(); i++){
			Ref<VDStatRelation> rel = rels[i];
			float calc_val = rel->_calculate(value);
			Array calc_vals;
			calc_vals.append(calc_val);
			calc_vals.append(rel);
			new_vals[rel->get_related_stat_name()] = calc_vals;
		}
	}
	return new_vals;
}

void VDStatsMeta::set_stat_relations(Dictionary relations){
	this->stat_relations = relations;
}

Dictionary VDStatsMeta::get_stat_relations() const {
	return this->stat_relations;
}