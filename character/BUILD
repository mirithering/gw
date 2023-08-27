cc_library(
    name = "character",
    srcs = [
        "action.cpp",
        "creature.cpp",
        "damage.cpp",
        "skill.cpp",
    ],
    hdrs = [
        "action.h",
        "build.h",
        "condition.h",
        "creature.h",
        "damage.h",
        "skill.h",
        "stance.h",
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
    ],
    deps = [
        "character",
        "//skills",
        "@com_google_googletest//:gtest_main",
    ],
)
