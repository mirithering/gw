cc_library(
    name = "skills",
    srcs = [
        "bonettis_defense.cpp",
    ],
    hdrs = [
        "attack/attack_skill.h",
        "attack/dagger/desperate_strike.h",
        "attack/sword/barbarous_slice.h",
        "attack/sword/gash.h",
        "attack/sword/jaizenju_strike.h",
        "attack/sword/pure_strike.h",
        "bonettis_defense.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//character",
        "//conditions",
    ],
)

cc_test(
    name = "test",
    size = "small",
    srcs = [
        "attack/sword/barbarous_slice_test.cpp",
        "attack/sword/gash_test.cpp",
        "attack/sword/pure_strike_test.cpp",
        "bonettis_defense_test.cpp",
    ],
    deps = [
        "skills",
        "//character",
        "@com_google_googletest//:gtest_main",
    ],
)
