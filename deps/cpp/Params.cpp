#include "Params.h"
#include <cstdlib>
#include <iostream>
#include <filesystem>

using namespace std;

vector<string> Split(const string &s, const char &c)
{
	string buff{""};
	vector<string> v;

	for (auto n : s)
	{
		if (n != c)
			buff += n;
		else if (n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		v.push_back(buff);

	return v;
}

Params::Params(string Instance_evalution, string inputFolderPath)
{

	vector<string> DataConfig = Split(Instance_evalution, '_');
	cityName = DataConfig[0];
	//cityName = string(argv[1]).c_str();
	cout << "CITY NAME: " << cityName << endl;

	targetSizeOfDistrict = stoi(DataConfig[3]);

	depotPosition = DataConfig[1];

	sizeTrainingSet =  stoi(DataConfig[4]);
	cout << "NB Scenarios PER Block: " << sizeTrainingSet << endl;
    
	
	instanceSize  = stoi(DataConfig[2]);
	int randomNumber = rand();
	seed = randomNumber % 10;
	generator.seed(seed);
	cout << "SEED Scenarios: " << seed << endl;

	// Define data paths and read datafile in JSON format
	instanceName = inputFolderPath + cityName;
	readBlocksJSON();

	probaCustomerDemandPerTargetSizeDistrict = map<int, double> {};//{3, 0.004}, {6, 0.002}, {12, 0.001}, {20, 0.0006}, {30, 0.0004}
	// check if the target size of district is in the map
	if (probaCustomerDemandPerTargetSizeDistrict.find(targetSizeOfDistrict) == probaCustomerDemandPerTargetSizeDistrict.end())
		// create a new entry in the map with value = 96/(8000*targetSizeOfDistrict)
		probaCustomerDemandPerTargetSizeDistrict[targetSizeOfDistrict] = 96. / (8000. * targetSizeOfDistrict);
	probaCustomerDemand = probaCustomerDemandPerTargetSizeDistrict[targetSizeOfDistrict];

	cout << "PROBA CUSTOMER DEMAND: " << probaCustomerDemand << endl;

	cout << "FINISHED READING DATA" << endl;

}

void Params::readBlocksJSON()
{
// Parsing all blocks in the JSON file
	ifstream inputFile(instanceName + ".geojson");
	if (inputFile.is_open())
	{
		json j;
		inputFile >> j;
		inputFile.close();
		int blockID = 0;
		for (auto &blockIN : j.at("features"))
		{
				
				Block block = Block();
				block.id = blockIN.at("properties").at("ID");
				//block.zone_name = blockIN.at("properties").at("NAME");
				block.nbInhabitants = blockIN.at("properties").at("POPULATION");
				auto &polyIN = blockIN.at("properties").at("POINTS");
				for (array<double, 2> longLatInfo : polyIN)
				{
					Point myPoint = {longLatInfo[0], longLatInfo[1]};
					block.verticesPoints.push_back(myPoint);
					block.minX = min<double>(myPoint.x, block.minX);
					block.maxX = max<double>(myPoint.x, block.maxX);
					block.minY = min<double>(myPoint.y, block.minY);
					block.maxY = max<double>(myPoint.y, block.maxY);
	
				}
				blocks.push_back(block);
		}

		referenceLongLat = j.at("metadata").at("REFERENCE_LONGLAT");
	}
	else
		throw std::invalid_argument("Impossible to open instance JSON file: " + instanceName + ".geojson");
}
void Params::exportBlockScenarios(string outputFolderPath)
{
		std::cout << "Exporting block scenarios to " << outputFolderPath << std::endl;
		vector<Block> currentBlocks = vector<Block> (blocks);
		currentBlocks.resize(instanceSize);

		maxX = -INFINITY;
		maxY = -INFINITY;

		minX = INFINITY;
		minY = INFINITY;

		for (Block block : currentBlocks)
		{
			maxX = max(maxX,block.maxX);
			maxY = max(maxY, block.maxY);

			minX = min(minX, block.minX);
			minY = min(minY, block.minY);
		}

		//depotPosition = "C";
          
		Point depotPoint;


		if (depotPosition == "C")  depotPoint = {(minX + maxX) / 2.0, (minY + maxY) / 2.0 };
		if (depotPosition == "NW") depotPoint = { minX, maxY };
		if (depotPosition == "NE") depotPoint = { maxX, maxY };
		if (depotPosition == "SW") depotPoint = { minX, minY };
		if (depotPosition == "SE") depotPoint = { maxX, minY };

			auto temp = wgs84::fromCartesian({referenceLongLat[1], referenceLongLat[0]}, {1000. * depotPoint.x, 1000. * depotPoint.y});
			array<double,2> depotLongLat = array<double,2> ({temp[1], temp[0]});

				double probaCustomer = probaCustomerDemandPerTargetSizeDistrict[targetSizeOfDistrict];
				
				int maxSizeOfDistrict = ceil(1.2 * targetSizeOfDistrict);
				int minSizeOfDistrict = floor(0.8 * targetSizeOfDistrict);

				int numberOfDistricts = floor(instanceSize / targetSizeOfDistrict);

				string instanceName = cityName + "_" + depotPosition + "_" + to_string(instanceSize) + "_" + to_string(targetSizeOfDistrict);

				string cityName = outputFolderPath + instanceName + ".json";
				// Check if the directory exists, and create it if it doesn't
				std::filesystem::path outputPath(cityName);
				std::filesystem::path outputDir = outputPath.parent_path();
				
				if (!std::filesystem::exists(outputPath.parent_path())) {
						std::filesystem::create_directories(outputPath.parent_path());
					}
				ofstream myfile;
					myfile.open(cityName);
					if (myfile.is_open())
					{
						json jblocks;
						for (Block block : currentBlocks)
						{
							json jBlock = {
								{"ID", block.id},
								{"Scenarios", block.trainScenarios}
							};
							jblocks += jBlock;
						}	

						myfile << json{{"blocks", jblocks},
									   {"metadata", {
										   	{"TARGET_SIZE_DISTRICT", targetSizeOfDistrict}, 
									   		{"MAX_SIZE_DISTRICT", maxSizeOfDistrict},
											{"MIN_SIZE_DISTRICT", minSizeOfDistrict},
											{"NUMBER_OF_DISTRICTS", numberOfDistricts},
											{"DEPOT_XY", depotPoint},
											{"DEPOT_LONGLAT", depotLongLat},
											{"PROBA_CUSTOMER_DEMAND", probaCustomer},
									   }}};
						myfile.close();
						cout << "Instance " << instanceName << " created" << endl;
					}
					else
						throw std::invalid_argument("Impossible to open output file: " + cityName);

}