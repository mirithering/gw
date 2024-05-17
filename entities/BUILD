cc_library(
    name = "entities",
    srcs = [
        "action.cpp",
        "creature.cpp",
        "damage.cpp",
        "effect.cpp",
        "skill.cpp",
        "stance.cpp",
    ],
    hdrs = [
        "action.h",
        "build.h",
        "condition.h",
        "creature.h",
        "damage.h",
        "effect.h",
        "hex.h",
        "skill.h",
        "stance.h",
        "world.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//armor",
        "//base",
        "//weapon",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "action_test.cpp",
        "build_test.cpp",
        "creature_test.cpp",
        "damage_test.cpp",
        "effect_test.cpp",
    ],
    deps = [
        "entities",
        "//skills",
        "//test",
        "@com_google_googletest//:gtest_main",
    ],
)
