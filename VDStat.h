#ifndef VDSTAT_H
#define VDSTAT_H

#include "core/reference.h"
#include "VDStatModifier.h"
#include "../vdcore/VDComponent.h"

class VDScStats;

class VDScStat : public Reference {
    GDCLASS(VDScStat, Reference);

    friend class VDScStats;

    // TODO: temporary save proccessed data in order
    // to make modifiers purely as Resource

    String name;
    float base_value = 0.0;
    float modificated_value = 0.0;
    Ref<VDScStats> owner;

    Vector<Ref<VDScStatModifier>> modifiers_regulars;
    Vector<Ref<VDScStatModifier>> modifiers_finals;
    HashMap<String, Ref<VDScStatModifier>> modifiers;

    float recalculate();
    void handle_modifier_value_changed(float new_value, float old_value, Ref<VDScStatModifier> modifier);

protected:
    static void _bind_methods();
public:
    VDScStat();

    void apply_modifier(Ref<VDScStatModifier> modifier, String modifier_name);
    Ref<VDScStatModifier> discard_modifier(String modifier_name, bool recalculate_after = true);

    void set_stat_name(String name);
    String get_stat_name() const;
    Ref<VDScStats> get_owner() const;

    void set_base_value(float value);
    float get_base_value() const;
    Ref<VDScStatModifier> get_modifier(String modifier) const;
    float get_modifier_value(String modifier) const;
    float get_modificated_value() const;
};

class VDScStats : public VDCcComponent {
    GDCLASS(VDScStats, VDCcComponent);

    friend class VDScStat;
    void notify_stat_change(Ref<VDScStat> stat, float new_value, float old_value = 0);
protected:
    HashMap<String, Ref<VDScStat>> stats;

    static void _bind_methods();
    virtual void finalize() override;

public:
    VDScStats();

    void add_stat(Ref<VDScStat> stat);
    Ref<VDScStat> get_stat(String attribute_name) const;
    bool has_stat(String attribute_name) const;
    Ref<VDScStat> remove_stat(String attribute_name);
    Array get_stats();

    // Nette Damage-Formulas
    // Nr.1	attack*(100/(100+defense))	es wird immer Schaden angerichtet, Schadensreduzierung verringert sich bei hoher Diskrepanz
    // Bsp.:	200 Dmg -> 10 Defense: 181 damage, 100 Defense: 100 damage, 200 Defense: 66 damage, 500 Defense: 33 damage
};

#endif
