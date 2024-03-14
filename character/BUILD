cc_library(
    name = "character",
    srcs = [
        "action.cpp",
        "character.cpp",
        "damage.cpp",
        "skill.cpp",
        "stance.cpp",
    ],
    hdrs = [
        "action.h",
        "build.h",
        "character.h",
        "condition.h",
        "damage.h",
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
        "character_test.cpp",
        "damage_test.cpp",
    ],
    deps = [
        "character",
        "//skills",
        "//test",
        "@com_google_googletest//:gtest_main",
    ],
)
