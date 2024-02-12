#include "Params.h"
#include "main.h"
#include <jlcxx/jlcxx.hpp>

int scmain(string Instance_evalution, string inputFolderPath, string outputFolderPath)
{
	/* READ INSTANCE FILE AND INITIALIZE DATA STRUCTURES */
	Params Params(Instance_evalution, inputFolderPath);

	/* FOR EACH DISTRICT SELECTED IN THE TRAINING SET */
		for (int b = 0; b < Params.blocks.size(); b++)
		{	
			vector<vector<Point>> trainScenarios = vector<vector<Point>>();
		
			for (int s = 0; s < Params.sizeTrainingSet; s++)
			{
				// Pick the number of points in the BlockSC following a binomial distribution based on the number of inhabitants
				vector<Point> evaluationPoints;
				binomial_distribution<> distB(Params.blocks[b].nbInhabitants, Params.probaCustomerDemand);
				int nbCustomersSampledBlock = distB(Params.generator);

				for (int i = 0; i < nbCustomersSampledBlock; i++)
				{
					uniform_real_distribution<> distX(Params.blocks[b].minX, Params.blocks[b].maxX);
					uniform_real_distribution<> distY(Params.blocks[b].minY, Params.blocks[b].maxY);

					Point randomPoint = {distX(Params.generator), distY(Params.generator)};

					while (!isInside(Params.blocks[b].verticesPoints, randomPoint))
						randomPoint = {distX(Params.generator), distY(Params.generator)};
					
					evaluationPoints.push_back(randomPoint);
				}

				trainScenarios.push_back(evaluationPoints);
			}

			Params.blocks[b].trainScenarios = trainScenarios;
		}
	/* EXPORT DISTRICTS TO FILE */
	Params.exportBlockScenarios(outputFolderPath);

    /* end*/
    return 0;
}


// Wrap the function for Julia
JLCXX_MODULE define_julia_module(jlcxx::Module& module) {
	module.method("SCmain", &scmain);
}