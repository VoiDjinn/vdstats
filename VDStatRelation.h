#ifndef VDSTATRELATION_H
#define VDSTATRELATION_H

#include "core/reference.h"

class VDStatRelation : public Reference {
	GDCLASS(VDStatRelation, Reference);
	
	protected:
		static void _bind_methods();
	 
		String related_stat_name;
		float related_value;
		bool adjusting_base;
		
	public:
		VDStatRelation(String related_stat_name = "", float related_value = 0, bool adjusting_base = true);

		virtual float _calculate(float value);
		
		void set_related_stat_name(String stat_name);
		String get_related_stat_name() const;
		void set_related_value(float value);
		float get_related_value() const;
		void set_is_adjusting_base(bool adjusting_base);
		bool is_adjusting_base() const;
		
		// #func _calculate(old_val : float, cur_val : float, new_val : float) -> float:
		// #	var result = 0.0
		// #	#PROBLEM: Aktueller Wert entspricht neuem Wert, aher mathematischer Fehler -> Untersuche das
		// #	#var cur_calc = (old_val - cur_val) * amount
		// #	var new_calc = new_val * amount
		// #	#result = cur_calc + new_calc
		// #	#print("Calculated for ", affected_stat, ": old-val - ", old_val, ", cur-val - ", cur_val, ", new-val - ", new_val, ", calculation: ", (cur_val * amount - old_val * amount), ", calculation (new val): ", new_val * amount, " = ", result)
		// #	return new_calc
};

#endif