#ifndef VDSTATSTEMPLATE_H
#define VDSTATSTEMPLATE_H

#include "core/resource.h"

class VDStatsTemplate : public Resource {
	GDCLASS(VDStatsTemplate, Resource);
	
	protected:
		static void _bind_methods();
	 
		Dictionary stat_templates;
	public:
		VDStatsTemplate();
		
		void init_by_entry(Dictionary entry);
		
		void set_stat_templates(Dictionary templates);
		Dictionary get_stat_templates() const;
};

#endif