#include "register_types.h"
#include "core/class_db.h"

#include "VDStat.h"
#include "VDFlatModifier.h"
#include "VDStatDependentModifier.h"

void register_vdstats_types(){

	ClassDB::register_class<VDScStats>();
	ClassDB::register_class<VDScStat>();
	ClassDB::register_class<VDScStatModifier>();
	ClassDB::register_class<VDScFlatModifier>();
	ClassDB::register_class<VDScStatDependentModifier>();
}

void unregister_vdstats_types() {}



