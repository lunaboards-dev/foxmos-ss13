local ffi = require("ffi")
ffi.cdef [[
char * enable_monstermos();
union ByondValueData {
	uint32_t ref;
	float num;
};

struct CByondValue {
	uint8_t type;
	uint8_t junk1, junk2, junk3;
	uint32_t data;
};
struct CByondValue fm_FUCK(int argc, struct CByondValue* argv);
]]
ffi.load("../foxmos.so")
--print(ffi.C.enable_monstermos)
print(ffi.C.fm_FUCK)
print("OK")