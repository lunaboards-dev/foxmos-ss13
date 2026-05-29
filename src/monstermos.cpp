#include "monstermos.h"

#include "GasMixture.h"
#include "turf_grid.h"
#include "utils.h"

#include <cmath>
#include <chrono>
#include <unordered_map>
#include <vector>

using namespace monstermos::constants;

/*trvh fuck(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return (const trvh) ByondValue("fuck");
}*/

std::unordered_map<std::string, ByondValue> gas_types;
std::unordered_map<unsigned int, int> gas_ids;
//std::unordered_map<unsigned int, std::shared_ptr<GasMixture>> gas_mixtures;
std::vector<ByondValue> gas_id_to_type;
TurfGrid all_turfs;
ByondValue SSair;
int str_id_extools_pointer;
int gas_mixture_count = 0;
//float gas_moles_visible[TOTAL_NUM_GASES];
std::vector<float> gas_moles_visible;
std::vector<std::vector<ByondValue>> gas_overlays;

std::shared_ptr<GasMixture> &get_gas_mixture(ByondValue &val)
{
	uint32_t v = (uint32_t)val.ReadVarByStrId(str_id_extools_pointer).GetNum();//val.get_by_id(str_id_extools_pointer).ByondValue;
	if (v == 0) Runtime("Gas mixture has null extools pointer");
	return *((std::shared_ptr<GasMixture>*)v);
}

#define MM_API(name, code) BYOND_EXPORT CByondValue fm_##name(u4c _argc, CByondValue argv[]) { \
	CByondValue src = argv[0]; \
	CByondValue * args = argv+1; \
	u4c argc = _argc - 1; \
	code; \
	return ByondNull; \
}

MM_API(FUCK, {
	return FromStr("FUCK");
})

int str_id_volume;
MM_API(gm_register, {
	std::shared_ptr<GasMixture> *ptr = new std::shared_ptr<GasMixture>;
	float volume = Byond_ReadVarByStrId(src, str_id_volume).GetNum();
	*ptr = std::make_shared<GasMixture>(volume);
	Byond_WriteVarByStrId(src, str_id_extools_pointer, ByondValue(NUMBER, (int)ptr));
	gas_mixture_count++;
})

/*int str_id_volume;
trvh gasmixture_register(unsigned int args_len, ByondValue* args, ByondValue src)
{
	//gas_mixtures[src.ByondValue] = std::make_shared<GasMixture>(src.get_by_id(str_id_volume).ByondValuef);
	std::shared_ptr<GasMixture> *ptr = new std::shared_ptr<GasMixture>;
	//*ptr = std::make_shared<GasMixture>(src.get_by_id(str_id_volume).ByondValuef);
	auto _volume = src.ReadVarByStrId(str_id_volume);
	float volume = _volume.GetNum();
	*ptr = std::make_shared<GasMixture>(volume);
	Byond_WriteVarByStrId(src, str_id_extools_pointer, ByondValue(NUMBER, (int)ptr));
	//SetVariable(src.type, src.ByondValue, str_id_extools_pointer, ByondValue(NUMBER, (int)ptr));
	gas_mixture_count++;
	return ByondNull;
}*/

MM_API(gm_unregister, {
	uint32_t v = Byond_ReadVarByStrId(src, str_id_extools_pointer).GetRef();
	if (v != 0) {
		std::shared_ptr<GasMixture> * gm = (std::shared_ptr<GasMixture> *)v;
		delete gm;
		gas_mixture_count--;
		Byond_WriteVarByStrId(src, str_id_extools_pointer, ByondNull);
	}
})

/*trvh gasmixture_unregister(unsigned int args_len, ByondValue* args, ByondValue src)
{
	uint32_t v = src.ReadVarByStrId(str_id_extools_pointer).GetRef();//src.get_by_id(str_id_extools_pointer).ByondValue;
	if (v != 0) {
		std::shared_ptr<GasMixture> *gm = (std::shared_ptr<GasMixture> *)v;
		delete gm;
		gas_mixture_count--;
		//SetVariable(src.type, src.ByondValue, str_id_extools_pointer, ByondValue::Null());
		Byond_WriteVarByStrId(src, str_id_extools_pointer, ByondNull);
	}
	return ByondNull;
}*/

// we'll figure you out later
//DelDatumPtr oDelDatum;
void hDelDatum(unsigned int datum_id) {
	/*RawDatum *datum = Core::GetDatumPointerById(datum_id);
	if (datum != nullptr) {
		std::shared_ptr<GasMixture> *gm = nullptr;
		if (datum->len_vars < 10) { // if it has a whole bunch of vars it's probably not a gas mixture. Please don't add a whole bunch of vars to gas mixtures.
			for (int i = 0; i < datum->len_vars; i++) {
				if (datum->vars[i].id == str_id_extools_pointer) {
					gm = (std::shared_ptr<GasMixture> *)datum->vars[i].ByondValue.ByondValue;
					datum->vars[i].ByondValue = ByondValue::Null();
					break;
				}
			}
		}
		if (gm != nullptr) {
			delete gm;
			gas_mixture_count--;
		}
	}
	oDelDatum(datum_id);*/
}

MM_API(gm_heat_cap, {
	return FromFloat(get_gas_mixture(src)->heat_capacity());
})

/*trvh gasmixture_heat_capacity(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return (CByondValue) FromFloat(get_gas_mixture(src)->heat_capacity());
	//return ByondValue(get_gas_mixture(src)->heat_capacity());
}*/

MM_API(gm_set_min_heat_cap, {
	get_gas_mixture(src)->set_min_heat_capacity(argc > 0 ? args[0].data.num : 0);
})

/*trvh gasmixture_set_min_heat_capacity(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->set_min_heat_capacity(args_len > 0 ? args[0].GetNum() : 0);
	return ByondNull;
}*/

MM_API(gm_total_moles, {
	return FromFloat(get_gas_mixture(src)->total_moles());
})

/*trvh gasmixture_total_moles(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->total_moles());
}*/

MM_API(gm_return_pressure, {
	return FromFloat(get_gas_mixture(src)->return_pressure());
})

/*trvh gasmixture_return_pressure(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->return_pressure());
}*/

MM_API(gm_return_temp, {
	return FromFloat(get_gas_mixture(src)->get_temperature());
})

/*trvh gasmixture_return_temperature(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->get_temperature());
}*/

MM_API(gm_return_vol, {
	return FromFloat(get_gas_mixture(src)->get_volume());
}) 

/* trvh gasmixture_return_volume(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->get_volume());
} */

MM_API(gm_thermal_energy, {
	return FromFloat(get_gas_mixture(src)->thermal_energy());
})

/* trvh gasmixture_thermal_energy(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->thermal_energy());
} */

MM_API(gm_archive, {
	get_gas_mixture(src)->archive();
})

/* trvh gasmixture_archive(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->archive();
	return ByondValue::Null();
} */

MM_API(gm_merge, {
	if (argc < 1) return ByondNull; // shouldn't this runtime?
	get_gas_mixture(src)->merge(*get_gas_mixture(args[0]));
})

/* trvh gasmixture_merge(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 1)
		return ByondValue::Null();
	get_gas_mixture(src)->merge(*get_gas_mixture(args[0]));
	return ByondValue::Null();
} */

MM_API(gm_remove_ratio, {
	if (argc < 2) return ByondNull;
	get_gas_mixture(args[0])->copy_from_mutable(get_gas_mixture(src)->remove_ratio(args[1].data.num));
})

/* trvh gasmixture_remove_ratio(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 2)
		return ByondValue::Null();
	get_gas_mixture(args[0])->copy_from_mutable(get_gas_mixture(src)->remove_ratio(args[1].ByondValuef));
	return ByondValue::Null();
} */

MM_API(gm_remove, {
	if (argc < 2) return ByondNull;
	get_gas_mixture(args[0])->copy_from_mutable(get_gas_mixture(src)->remove(args[1].data.num));
})

/* trvh gasmixture_remove(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 2)
		return ByondValue::Null();
	get_gas_mixture(args[0])->copy_from_mutable(get_gas_mixture(src)->remove(args[1].ByondValuef));
	return ByondValue::Null();
} */

MM_API(gm_copy_from, {
	if (argc < 1) return ByondNull;
	get_gas_mixture(src)->copy_from_mutable(*get_gas_mixture(args[0]));
})

/* trvh gasmixture_copy_from(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 1)
		return ByondValue::Null();
	get_gas_mixture(src)->copy_from_mutable(*get_gas_mixture(args[0]));
	return ByondValue::Null();
} */

MM_API(gm_share, {
	if (argc < 1) return ByondNull;
	return FromFloat(get_gas_mixture(src)->share(*get_gas_mixture(args[0]), argc >= 2 ? args[1].data.num : 4));
})

/* trvh gasmixture_share(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 1)
		return ByondValue::Null();
	ByondValue ret = ByondValue(get_gas_mixture(src)->share(*get_gas_mixture(args[0]), args_len >= 2 ? args[1].ByondValuef : 4));
	return ret;
} */

MM_API(gm_last_share, {
	return FromFloat(get_gas_mixture(src)->get_last_share());
})

/* trvh gasmixture_get_last_share(unsigned int args_len, ByondValue* args, ByondValue src)
{
	return ByondValue(get_gas_mixture(src)->get_last_share());
} */

MM_API(gm_get_gasses, {
	CByondValue list;
	GasMixture &gm = *get_gas_mixture(src);
	Byond_CreateList(&list);
	int count = 0;
	for (int i = 0; i < total_num_gases; i++) {
		if (gm.get_moles(i) >= GAS_MIN_MOLES) {
			Byond_WriteListIndex(list, FromFloat(count++), gas_id_to_type[i]);
		}
	}
	return list;
})

/* trvh gasmixture_get_gases(unsigned int args_len, ByondValue* args, ByondValue src)
{
	List l(CreateList(0));
	GasMixture &gm = *get_gas_mixture(src);
	for (int i = 0; i < TOTAL_NUM_GASES; i++) {
		if (gm.get_moles(i) >= GAS_MIN_MOLES) {
			l.append(gas_id_to_type[i]);
		}
	}
	return l;
} */

MM_API(gm_set_temp, {
	float vf = argc > 0 ? args[0].data.num : 0;
	if (std::isnan(vf) || std::isinf(vf)) {
		Runtime("Attempt to set temperature to NaN or Infinity");
	} else {
		get_gas_mixture(src)->set_temperature(vf);
	}
})

/* trvh gasmixture_set_temperature(unsigned int args_len, ByondValue* args, ByondValue src)
{
	float vf = args_len > 0 ? args[0].ByondValuef : 0;
	if (std::isnan(vf) || std::isinf(vf)) {
		Runtime("Attempt to set temperature to NaN or Infinity");
	} else {
		get_gas_mixture(src)->set_temperature(vf);
	}
	return ByondValue::Null();
} */

MM_API(gm_set_volume, {
	get_gas_mixture(src)->set_volume(argc > 0 ? args[0].data.num : 0);
})

/* trvh gasmixture_set_volume(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->set_volume(args_len > 0 ? args[0].ByondValuef : 0);
	return ByondValue::Null();
} */

MM_API(gm_get_moles, {
	if (argc < 1 || args[0].type != DATUM_TYPEPATH) return ByondNull;
	int index = gas_ids[args[0].data.ref];
	return FromFloat(get_gas_mixture(src)->get_moles(index));
})

/* trvh gasmixture_get_moles(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 1 || args[0].type != DATUM_TYPEPATH)
		return ByondValue::Null();
	int index = gas_ids[args[0].ByondValue];
	return ByondValue(get_gas_mixture(src)->get_moles(index));
} */

MM_API(gm_set_moles, {
	if (argc < 2 || args[0].type != DATUM_TYPEPATH) return ByondNull;
	int index = gas_ids[args[0].data.ref];
	get_gas_mixture(src)->set_moles(index, args[1].data.num);
})

/* trvh gasmixture_set_moles(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 2 || args[0].type != DATUM_TYPEPATH)
		return ByondValue::Null();
	int index = gas_ids[args[0].ByondValue];
	get_gas_mixture(src)->set_moles(index, args[1].ByondValuef);
	return ByondValue::Null();
} */

MM_API(gm_scrub_into, {
	if (argc < 2) return ByondNull;
	GasMixture &src_gas = *get_gas_mixture(src);
	GasMixture &dest_gas = *get_gas_mixture(args[0]);
	CByondValue gases_to_scrub = args[1];
	u4c num_gases = 0;
	Byond_ReadList(&gases_to_scrub, (CByondValue*)nullptr, &num_gases);
	CByondValue gases[num_gases]; 
	Byond_ReadList(&gases_to_scrub, gases, &num_gases);
	GasMixture buffer(CELL_VOLUME);
	buffer.set_temperature(src_gas.get_temperature());
	for (int i = 0; i < num_gases; i++) {
		CByondValue typepath = gases[i];
		if (typepath.type != DATUM_TYPEPATH) continue;
		int index = gas_ids[typepath.data.ref];
		buffer.set_moles(index, buffer.get_moles(index) + src_gas.get_moles(index));
		src_gas.set_moles(index, 0);
	}
	dest_gas.merge(buffer);
	return args[0];
})

/* trvh gasmixture_scrub_into(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 2)
		return ByondValue::Null();
	GasMixture &src_gas = *get_gas_mixture(src);
	GasMixture &dest_gas = *get_gas_mixture(args[0]);
	Container gases_to_scrub = args[1];
	int num_gases = gases_to_scrub.length();
	GasMixture buffer(CELL_VOLUME);
	buffer.set_temperature(src_gas.get_temperature());
	for (int i = 0; i < num_gases; i++) {
		ByondValue typepath = gases_to_scrub[i];
		if (typepath.type != DATUM_TYPEPATH) continue;
		int index = gas_ids[typepath.ByondValue];
		buffer.set_moles(index, buffer.get_moles(index) + src_gas.get_moles(index));
		src_gas.set_moles(index, 0);
	}
	dest_gas.merge(buffer);
	IncRefCount(args[0].type, args[0].ByondValue);
	return args[0];
} */

MM_API(gm_mark_immutable, {
	get_gas_mixture(src)->mark_immutable();
})

/* trvh gasmixture_mark_immutable(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->mark_immutable();
	return ByondValue::Null();
} */

MM_API(gm_mark_vacuum, {
	get_gas_mixture(src)->mark_vacuum();
})

/* trvh gas_mixture_mark_vacuum(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->mark_vacuum();
	return ByondValue::Null();
} */

MM_API(gm_clear, {
	get_gas_mixture(src)->clear();
})

/* trvh gasmixture_clear(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->clear();
	return ByondValue::Null();
} */

MM_API(gm_compare, {
	if (argc < 1) return ByondNull;
	int result = get_gas_mixture(src)->compare(*get_gas_mixture(args[0]));
	if (result == -1) {
		return FromStr("temp");
	}
	else if (result == -2) {
		return FromStr("");
	} else{
		return gas_id_to_type[result].value;
	}
})

/* trvh gasmixture_compare(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (args_len < 1)
		return ByondValue::Null();
	int result = get_gas_mixture(src)->compare(*get_gas_mixture(args[0]));
	if (result == -1) {
		return ByondValue("temp");
	}
	else if (result == -2) {
		return ByondValue("");
	} else{
		return gas_id_to_type[result];
	}
} */

MM_API(gm_multiply, {
	get_gas_mixture(src)->multiply(argc > 0 ? args[0].data.num : 1);
})

/* trvh gasmixture_multiply(unsigned int args_len, ByondValue* args, ByondValue src)
{
	get_gas_mixture(src)->multiply(args_len > 0 ? args[0].ByondValuef : 1);
	return ByondValue::Null();
} */

MM_API(gm_create_temp_grad, {
	if (argc < 3) {
		return ByondFalse;
	} else if (args[0].type != NUMBER || args[1].type != NUMBER || args[2].type != NUMBER) {
		return ByondFalse;
	} else if (get_gas_mixture(src)->create_temperature_gradient(args[0].data.num, args[1].data.num, args[2].data.num)) {
		return ByondTrue;
	} else {
		return ByondFalse;
	}
})

/* trvh gasmixture_create_temperature_gradient(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if(args_len < 3) {
		return ByondValue::False();
	} else if(args[0].type != NUMBER || args[1].type != NUMBER || args[2].type != NUMBER) {
		return ByondValue::False();
	} else if (get_gas_mixture(src)->create_temperature_gradient(args[0].ByondValuef, args[1].ByondValuef, args[2].ByondValuef)) {
		return ByondValue::True();
	} else {
		return ByondValue::False();
	}
} */

MM_API(gm_tick_temp_grad, {
	if (argc < 1 || args[0].type != NUMBER) return ByondNull;
	get_gas_mixture(src)->tick_temperature_gradient(args[0].data.num);
})

/* trvh gasmixture_tick_temperature_gradient(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if(args_len < 1) { return ByondValue::Null(); }
	else if(args[0].type != NUMBER) { return ByondValue::Null(); }
	else {
		get_gas_mixture(src)->tick_temperature_gradient(args[0].ByondValuef);
		return ByondValue::Null();
	}
} */

#define MM_TURF_API(name, code) MM_API(turf_##name, { \
	if (src.type != TURF) return ByondNull; \
	Tile *tile = all_turfs.get(src.data.ref); \
	code \
})

MM_TURF_API(update_adjacent, {
	if (tile != nullptr) tile->update_adjacent(all_turfs);
})

/* trvh turf_update_adjacent(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		tile->update_adjacent(all_turfs);
	}
	return ByondValue::Null();
} */

MM_TURF_API(update_air_ref, {
	if (tile != nullptr) tile->update_air_ref();
})

/* trvh turf_update_air_ref(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		tile->update_air_ref();
	}
	return ByondValue::Null();
} */

MM_TURF_API(eg_reset_cooldowns, {
	if (tile != nullptr && tile->excited_group) tile->excited_group->reset_cooldowns();
})

/* trvh turf_eg_reset_cooldowns(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		if (tile->excited_group) {
			tile->excited_group->reset_cooldowns();
		}
	}
	return ByondValue::Null();
} */

MM_TURF_API(eg_garbage_collect, {
	if (tile != nullptr) {
		if (tile->excited_group) {
			std::shared_ptr<ExcitedGroup> eg = tile->excited_group;
			eg->dismantle(false);
		}
	}
})

/* trvh turf_eg_garbage_collect(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		if (tile->excited_group) {
			// store to local variable to prevent it from being destructed while we're still using it because that causes segfaults.
			std::shared_ptr<ExcitedGroup> eg = tile->excited_group;
			eg->dismantle(false);
		}
	}
	return ByondValue::Null();
} */

MM_TURF_API(get_excited, {
	if (tile != nullptr) return tile->excited ? ByondTrue : ByondFalse;
})

/* trvh turf_get_excited(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		return ByondValue(tile->excited ? 1.0 : 0.0);
	}
	return ByondValue::Null();
} */

MM_TURF_API(set_excited, {
	if (tile != nullptr) tile->excited = argc > 0 ? args[0].data.ref : false;
})

/* trvh turf_set_excited(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		tile->excited = args_len > 0 ? (bool)args[0] : false;
	}
	return ByondValue::Null();
} */

MM_TURF_API(process_cell, {
	if (tile != nullptr && argc > 0) tile->process_cell(args[0].data.num);
})

/* trvh turf_process_cell(unsigned int args_len, ByondValue* args, ByondValue src)
{
	if (src.type != TURF || args_len < 1) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		tile->process_cell(args[0]);
	}
	return ByondValue::Null();
} */

MM_TURF_API(eq, {
	if (tile != nullptr && argc > 0) tile->equalize_pressure_in_zone(args[0].data.num);
})

/* trvh turf_eq(unsigned int args_len, ByondValue* args, ByondValue src) {
	if (src.type != TURF || args_len < 1) { return ByondValue::Null(); }
	Tile *tile = all_turfs.get(src.ByondValue);
	if (tile != nullptr) {
		tile->equalize_pressure_in_zone(args[0]);
	}
	return ByondValue::Null();
} */

unsigned int str_id_atmos_overlay_types;
// may god have mercy
/*MM_TURF_API(update_visuals, {
	
})*/

// for whatever reason, i couldn't use the macros for this one.
BYOND_EXPORT CByondValue mm_turf_update_visuals(u4c _argc, CByondValue argv[]) {
	CByondValue src = argv[0];
	CByondValue * args = argv+1;
	u4c argc = _argc - 1;
	if (src.type != TURF) return ByondNull;
	Tile *tile = all_turfs.get(src.data.ref);
	if (!tile->air) return ByondNull;
	GasMixture& gm = *tile->air;
	ByondValue old_overlay_types_val = Byond_ReadVarByStrId(src, str_id_atmos_overlay_types);
	CByondValue old_overlay_types_cval = old_overlay_types_val.value;
	std::vector<ByondValue> overlay_types;

	for (int i = 0; i < total_num_gases; i++) {
		if (!gas_overlays[i].size()) continue;
		if (gm.get_moles(i) > gas_moles_visible[i]) {
			// you know whats fun?
			// getting cucked by BYOND arrays starting at 1. How did this not segfault before? Beats me! I love undefined behavior!    Bandaid: VV
			overlay_types.push_back(gas_overlays[i][std::fmin(FACTOR_GAS_VISIBLE_MAX, (int)std::ceil(gm.get_moles(i) / MOLES_GAS_VISIBLE_STEP))-1]);
		}
	}
	
	if (!overlay_types.size() && !old_overlay_types_val) return ByondNull;
	if (old_overlay_types_val) {
		u4c list_len = 0;
		Byond_ReadList(&old_overlay_types_cval, nullptr, &list_len);
		CByondValue old_overlay_types[list_len];
		Byond_ReadList(&old_overlay_types_cval, old_overlay_types, &list_len);
		if (overlay_types.size() == list_len) {
			bool is_different = false;
			for (int i = 0; i < overlay_types.size(); i++) {
				if (overlay_types[i] != old_overlay_types[i]) {
					is_different = true; break;
				}
			}
			if (!is_different) {
				return ByondNull;
			}
		}
	}
	CByondValue list;
	Byond_CreateListLen(&list, overlay_types.size());
	for (int i = 0; i < overlay_types.size(); i++) {
		//list.append(overlay_types[i]);
		Byond_WriteListIndex(list, FromFloat(i+1), overlay_types[i]);
	}
	Byond_CallProc(src, "set_visuals", &list, 1);
}

/* trvh turf_update_visuals(unsigned int args_len, ByondValue* args, ByondValue src) {
	if (src.type != TURF) { return ByondValue::Null(); }
	Tile* tile = all_turfs.get(src.ByondValue);
	if (!tile->air) return ByondValue::Null();
	GasMixture& gm = *tile->air;
	ByondValue old_overlay_types_val = src.get_by_id(str_id_atmos_overlay_types);
	std::vector<ByondValue> overlay_types;

	for (int i = 0; i < TOTAL_NUM_GASES; i++) {
		if (!gas_overlays[i].size()) continue;
		if (gm.get_moles(i) > gas_moles_visible[i]) {
			// you know whats fun?
			// getting cucked by BYOND arrays starting at 1. How did this not segfault before? Beats me! I love undefined behavior!    Bandaid: VV
			overlay_types.push_back(gas_overlays[i][std::fmin(FACTOR_GAS_VISIBLE_MAX, (int)std::ceil(gm.get_moles(i) / MOLES_GAS_VISIBLE_STEP))-1]);
		}
	}

	if (!overlay_types.size() && !old_overlay_types_val) return ByondValue::Null();
	if (old_overlay_types_val) {
		List old_overlay_types(old_overlay_types_val);
		if (overlay_types.size() == old_overlay_types.list->length) {
			bool is_different = false;
			for (int i = 0; i < overlay_types.size(); i++) {
				if (overlay_types[i] != old_overlay_types.at(i)) {
					is_different = true; break;
				}
			}
			if (!is_different) {
				return ByondValue::Null();
			}
		}
	}
	
	List l(CreateList(0));
	for (int i = 0; i < overlay_types.size(); i++) {
		l.append(overlay_types[i]);
	}
	src.invoke("set_visuals", { ByondValue(l) } );
	return ByondValue::Null();
} */

std::vector<std::weak_ptr<ExcitedGroup>> excited_groups_currentrun;
MM_API(ssair_process_excited_groups, {
	auto start = std::chrono::high_resolution_clock::now();
	float time_limit = args[1].data.num * 100000.0f;

	if (argc < 2) { return ByondNull; }
	if (!ByondValue_IsTrue(&args[0])) {
		excited_groups_currentrun = excited_groups; // this copies it.... right?
	}
	while (excited_groups_currentrun.size()) {
		std::shared_ptr<ExcitedGroup> eg = excited_groups_currentrun.back().lock();
		excited_groups_currentrun.pop_back();
		if (!eg) continue;
		eg->breakdown_cooldown++;
		eg->dismantle_cooldown++;
		if (eg->breakdown_cooldown >= EXCITED_GROUP_BREAKDOWN_CYCLES)
			eg->self_breakdown();
		if (eg->dismantle_cooldown >= EXCITED_GROUP_DISMANTLE_CYCLES)
			eg->dismantle(true);
		if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() > time_limit) {
			return ByondTrue;
		}
	}
	return ByondFalse;
})

/* trvh SSair_process_excited_groups(unsigned int args_len, ByondValue* args, ByondValue src) {
	auto start = std::chrono::high_resolution_clock::now();
	float time_limit = args[1] * 100000.0f;

	if (args_len < 2) { return ByondValue::Null(); }
	if (!args[0]) {
		excited_groups_currentrun = excited_groups; // this copies it.... right?
	}
	while (excited_groups_currentrun.size()) {
		std::shared_ptr<ExcitedGroup> eg = excited_groups_currentrun.back().lock();
		excited_groups_currentrun.pop_back();
		if (!eg) continue;
		eg->breakdown_cooldown++;
		eg->dismantle_cooldown++;
		if (eg->breakdown_cooldown >= EXCITED_GROUP_BREAKDOWN_CYCLES)
			eg->self_breakdown();
		if (eg->dismantle_cooldown >= EXCITED_GROUP_DISMANTLE_CYCLES)
			eg->dismantle(true);
		if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() > time_limit) {
			return ByondValue::True();
		}
	}
	return ByondValue::False();
} */

MM_API(ssair_get_amt_excited_groups, {
	return FromFloat(excited_groups.size());
})

/* trvh SSair_get_amt_excited_groups(unsigned int args_len, ByondValue* args, ByondValue src) {
	return ByondValue(excited_groups.size());
} */

MM_API(refresh_atmos_grid, {
	all_turfs.refresh();
})

/* trvh refresh_atmos_grid(unsigned int args_len, ByondValue* args, ByondValue src)
{
	all_turfs.refresh();
	return ByondValue::Null();
} */

void initialize_gas_overlays() {
	ByondValue GLOB = GetGlobal("GLOB");//ByondValue::Global().get("GLOB");
	if (!GLOB) return;
	//Container meta_gas_info = GLOB.get("meta_gas_info");
	ByondValue meta_gas_info = GLOB.ReadVar("meta_gas_info");
	if (!meta_gas_info) return;
	for (int i = 0; i < total_num_gases; ++i)
	{
		CByondValue v = gas_id_to_type[i].value;
		//Container gas_meta = meta_gas_info.at(v);
		ByondValue _gas_meta = Byond_ReadListIndex(meta_gas_info, v);
		u4c gm_len = 0;
		Byond_ReadList(_gas_meta, nullptr, &gm_len);
		ByondValue gas_meta[gm_len];
		Byond_ReadList(_gas_meta, gas_meta, &gm_len);
		gas_moles_visible[i] = gas_meta[2];
		gas_overlays[i].clear();
		if (gas_meta[3]) {
			//Container gas_overlays_list = gas_meta.at(3);
			CByondValue _gas_overlays_list = gas_meta[3].value;
			std::vector<ByondValue> gas_overlays_list = Byond_ReadList(_gas_overlays_list);
			int num_overlays = gas_overlays_list.size();
			for (int j = 0; j < num_overlays; j++) {
				gas_overlays[i].push_back(gas_overlays_list[j]);
			}
		}

	}
}

MM_API(ssair_update_ssair, {
	SSair = src;
	initialize_gas_overlays();
})

/* trvh SSair_update_ssair(unsigned int args_len, ByondValue* args, ByondValue src) {
	SSair = src;
	initialize_gas_overlays();
	return ByondValue::Null();
} */

int str_id_air;
int str_id_atmosadj;
int str_id_is_openturf;
int str_id_x, str_id_y, str_id_z;
int str_id_current_cycle, str_id_archived_cycle, str_id_planetary_atmos, str_id_initial_gas_mix;
int str_id_active_turfs;
int str_id_react, str_id_consider_pressure_difference, str_id_update_visuals, str_id_floor_rip;

const char* enable_monstermos()
{
	//oDelDatum = (DelDatumPtr)Core::install_hook((void*)DelDatum, (void*)hDelDatum);
	// get the var IDs for SANIC SPEED
	str_id_air = Byond_GetStrId("air");
	str_id_atmosadj = Byond_GetStrId("atmos_adjacent_turfs");
	str_id_volume = Byond_GetStrId("initial_volume");
	str_id_is_openturf = Byond_GetStrId("is_openturf");
	str_id_x = Byond_GetStrId("x");
	str_id_y = Byond_GetStrId("y");
	str_id_z = Byond_GetStrId("z");
	str_id_current_cycle = Byond_GetStrId("current_cycle");
	str_id_archived_cycle = Byond_GetStrId("archived_cycle");
	str_id_active_turfs = Byond_GetStrId("active_turfs");
	str_id_planetary_atmos = Byond_GetStrId("planetary_atmos");
	str_id_initial_gas_mix = Byond_GetStrId("initial_gas_mix");
	str_id_atmos_overlay_types = Byond_GetStrId("atmos_overlay_types");
	str_id_react = Byond_GetStrId("react");
	str_id_consider_pressure_difference = Byond_GetStrId("consider pressure difference"); // byond replaces "_" with " " in proc names. thanks BYOND.
	str_id_update_visuals = Byond_GetStrId("update visuals");
	str_id_floor_rip = Byond_GetStrId("handle decompression floor rip");
	str_id_extools_pointer = Byond_GetStrId("_extools_pointer_gasmixture");

	SSair = GetGlobal("SSair"); //ByondValue::Global().get("SSair");
	//Set up gas types map
	std::vector<ByondValue> nullvector = { ByondValue(0.0f) };
	//Container gas_types_list = Core::get_proc("/proc/gas_types").call(nullvector);
	CByondValue _gas_types_list;
	Byond_CallGlobalProc("/gas_types", nullptr, 0, &_gas_types_list);
	//Container meta_gas_info = ByondValue::Global().get("meta_gas_info");
	u4c gaslen;
	Byond_ReadList(&_gas_types_list, nullptr, &gaslen);
	total_num_gases = gaslen;
	gas_overlays.resize(gaslen);
	gas_specific_heat.resize(gaslen);
	gas_moles_visible.resize(gaslen);
	CByondValue gas_types_list[gaslen];
	Byond_ReadList(&_gas_types_list, gas_types_list, &gaslen);
	/*if (gaslen != TOTAL_NUM_GASES) {
		return "TOTAL_NUM_GASES does not match the number of /datum/gas subtypes!!";
	}*/
	for (int i = 0; i < gaslen; ++i)
	{
		CByondValue v = gas_types_list[i];
		std::string key = Byond_ToString(v);
		gas_types[key] = gas_types_list[i];
		gas_ids[v.data.ref] = i;
		gas_specific_heat[i] = gas_types_list[i].data.num;
		gas_id_to_type.push_back(v);
	}
	initialize_gas_overlays();
	//Set up hooks
	/*Core::get_proc("/datum/gas_mixture/proc/__gasmixture_register").hook(gasmixture_register);
	Core::get_proc("/datum/gas_mixture/proc/__gasmixture_unregister").hook(gasmixture_unregister);
	Core::get_proc("/datum/gas_mixture/proc/heat_capacity").hook(gasmixture_heat_capacity);
	Core::get_proc("/datum/gas_mixture/proc/set_min_heat_capacity").hook(gasmixture_set_min_heat_capacity);
	Core::get_proc("/datum/gas_mixture/proc/total_moles").hook(gasmixture_total_moles);
	Core::get_proc("/datum/gas_mixture/proc/return_pressure").hook(gasmixture_return_pressure);
	Core::get_proc("/datum/gas_mixture/proc/return_temperature").hook(gasmixture_return_temperature);
	Core::get_proc("/datum/gas_mixture/proc/return_volume").hook(gasmixture_return_volume);
	Core::get_proc("/datum/gas_mixture/proc/thermal_energy").hook(gasmixture_thermal_energy);
	Core::get_proc("/datum/gas_mixture/proc/archive").hook(gasmixture_archive);
	Core::get_proc("/datum/gas_mixture/proc/merge").hook(gasmixture_merge);
	Core::get_proc("/datum/gas_mixture/proc/copy_from").hook(gasmixture_copy_from);
	Core::get_proc("/datum/gas_mixture/proc/share").hook(gasmixture_share);
	Core::get_proc("/datum/gas_mixture/proc/compare").hook(gasmixture_compare);
	Core::get_proc("/datum/gas_mixture/proc/get_gases").hook(gasmixture_get_gases);
	Core::get_proc("/datum/gas_mixture/proc/__remove").hook(gasmixture_remove);
	Core::get_proc("/datum/gas_mixture/proc/__remove_ratio").hook(gasmixture_remove_ratio);
	Core::get_proc("/datum/gas_mixture/proc/set_temperature").hook(gasmixture_set_temperature);
	Core::get_proc("/datum/gas_mixture/proc/set_volume").hook(gasmixture_set_volume);
	Core::get_proc("/datum/gas_mixture/proc/get_moles").hook(gasmixture_get_moles);
	Core::get_proc("/datum/gas_mixture/proc/set_moles").hook(gasmixture_set_moles);
	Core::get_proc("/datum/gas_mixture/proc/scrub_into").hook(gasmixture_scrub_into);
	Core::get_proc("/datum/gas_mixture/proc/mark_immutable").hook(gasmixture_mark_immutable);
	Core::get_proc("/datum/gas_mixture/proc/mark_vacuum").hook(gas_mixture_mark_vacuum);
	Core::get_proc("/datum/gas_mixture/proc/clear").hook(gasmixture_clear);
	Core::get_proc("/datum/gas_mixture/proc/multiply").hook(gasmixture_multiply);
	Core::get_proc("/datum/gas_mixture/proc/create_temperature_gradient").hook(gasmixture_create_temperature_gradient);
	Core::get_proc("/datum/gas_mixture/proc/tick_temperature_gradient").hook(gasmixture_tick_temperature_gradient);
	Core::get_proc("/datum/gas_mixture/proc/get_last_share").hook(gasmixture_get_last_share);
	Core::get_proc("/turf/proc/__update_extools_adjacent_turfs").hook(turf_update_adjacent);
	Core::get_proc("/turf/proc/update_air_ref").hook(turf_update_air_ref);
	Core::get_proc("/turf/open/proc/eg_reset_cooldowns").hook(turf_eg_reset_cooldowns);
	Core::get_proc("/turf/open/proc/eg_garbage_collect").hook(turf_eg_garbage_collect);
	Core::get_proc("/turf/open/proc/get_excited").hook(turf_get_excited);
	Core::get_proc("/turf/open/proc/set_excited").hook(turf_set_excited);
	Core::get_proc("/turf/open/proc/process_cell").hook(turf_process_cell);
	Core::get_proc("/turf/open/proc/equalize_pressure_in_zone").hook(turf_eq);
	Core::get_proc("/turf/open/proc/update_visuals").hook(turf_update_visuals);
	Core::get_proc("/world/proc/refresh_atmos_grid").hook(refresh_atmos_grid);
	Core::get_proc("/datum/controller/subsystem/air/proc/process_excited_groups_extools").hook(SSair_process_excited_groups);
	Core::get_proc("/datum/controller/subsystem/air/proc/get_amt_excited_groups").hook(SSair_get_amt_excited_groups);
	Core::get_proc("/datum/controller/subsystem/air/proc/extools_update_ssair").hook(SSair_update_ssair);*/

	all_turfs.refresh();
	return "ok";
}
