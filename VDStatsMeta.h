#ifndef VDSTATSMETA_H
#define VDSTATSMETA_H

#include "core/reference.h"
#include "VDStatRelation.h"

class VDStatsMeta : public Reference {
	GDCLASS(VDStatsMeta, Reference);
	
	protected:
		static void _bind_methods();
	 
		float default_base_value  = 0.0;
		float default_attribute_modifier = 0.0;
		Dictionary stat_relations;
		
	public:
		VDStatsMeta();
		
		void add_relation(String source_attribute, String target_attribute, float value, bool adjusting_base = true);
		Dictionary calculate_relations(String source_attribute, float value) const;
		
		void set_stat_relations(Dictionary relations);
		Dictionary get_stat_relations() const;
		
		// #func calc_new_value(aff_stat, old_val : float, cur_val : float, new_val : float) -> Dictionary:
		// #	var new_vals : Array = {}
		// #	if stat_rels.has(aff_stat):
		// #		var rels : Array = stat_rels.get(aff_stat)
		// #		for rel in rels: new_vals[rel.affected_stat] = [rel._calculate(old_val, cur_val, new_val), rel]
		// #	return new_vals
};

#endif