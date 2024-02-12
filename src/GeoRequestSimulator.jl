module GeoRequestSimulator

    include("../deps/build.jl")
    include("utils.jl")
    using .CppScenario: SCmain
    

    export createScenario
    function createScenario(city, depot_location, nb_bu_large, district_size, nb_scenario)
        pathScenario = "Scenario/output/$(city)_$(depot_location)_$(nb_bu_large)_$(district_size).json"
    
        if isfile(pathScenario)
            rm(pathScenario)
        end
    
        StringInstance = city * "_C_" * string(nb_bu_large) * "_" * string(district_size) * "_" * string(nb_scenario)
        SCmain(StringInstance, "data/", "Scenario/output")
    end


end
