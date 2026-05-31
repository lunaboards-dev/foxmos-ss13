#define FOXMOS_PASS_AND_CALL(func) var/list/_temp = args.Copy(); _temp.Insert(1, src); return call_ext("../foxmos.so", func)(arglist(_temp))
/world/proc/mytest()
    world.log << length(args)

/world/proc/fuck()
    FOXMOS_PASS_AND_CALL("byond:fm_FUCK")

/world/New()
    world.log << fuck("a", "b", "c", "d")
    shutdown()