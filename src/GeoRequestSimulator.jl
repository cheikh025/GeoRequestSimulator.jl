module GeoRequestSimulator

    include("../deps/build.jl")
    using CxxWrap
    # Define a function that returns the library path
    function get_libGenSC_path()
        script_dir = @__DIR__
        return joinpath(script_dir, "../deps/cpp/build/libGenerateScenario.so")
    end
    script_dir = @__DIR__
    @wrapmodule(joinpath(script_dir, "../deps/cpp/build/libGenerateScenario.so"))

    function __init__()
        @initcxx
    end
    
    function createScenario(city, depot_location, nb_bu_large, district_size, nb_scenario)
        pathScenario = "Scenario/output/$(city)_$(depot_location)_$(nb_bu_large)_$(district_size).json"
    
        if isfile(pathScenario)
            rm(pathScenario)
        end
    
        StringInstance = city * "_C_" * string(nb_bu_large) * "_" * string(district_size) * "_" * string(nb_scenario)
        GeoRequestSimulator.SCmain(StringInstance, "data/", "Scenario/output/")
    end
    export createScenario


end
