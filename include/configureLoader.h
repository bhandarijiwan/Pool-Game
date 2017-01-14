/*
 * configureLoader.h
 *
 *  Created on: Oct 15, 2016
 *      Author: check
 */

#ifndef INCLUDE_CONFIGURELOADER_H_
#define INCLUDE_CONFIGURELOADER_H_
#include <fstream>
#include <string>
#include <vector>
using namespace std;
//configure planets' properties
class ConfigureLoader
{

public:
	static ConfigureLoader * GetInstance();
	float sunScale;

	float mercuryScale;
	float mercuryOrbitPeriod;
	float mercuryRotationPeriod;
	float mercuryOffset;

	float venusScale;
	float venusOrbitPeriod;
	float venusRotationPeriod;
	float venusOffset;

	float earthScale;
	float earthOrbitPeriod;
	float earthRotationPeriod;
	float earthOffset;
	float earthMajorOfOrbit;
	float earthRatioOfMinorToMajor;

	float moonScale;
	float moonOrbitPeriod;
	float moonRotationPeriod;
	float moonOffset;
	int moonOfPlanetIndex0;

	float marsScale;
	float marsOrbitPeriod;
	float marsRotationPeriod;
	float marsOffset;

	float jupiterScale;
	float jupiterOrbitPeriod;
	float jupiterRotationPeriod;
	float jupiterOffset;

	float saturnScale;
	float saturnOrbitPeriod;
	float saturnRotationPeriod;
	float saturnOffset;

	float uranusScale;
	float uranusOrbitPeriod;
	float uranusRotationPeriod;
	float uranusOffset;

	float neptuneScale;
	float neptuneOrbitPeriod;
	float neptuneRotationPeriod;
	float neptuneOffset;

	float plutoScale;
	float plutoOrbitPeriod;
	float plutoRotationPeriod;
	float plutoOffset;

	float distanceFactor;
	float sizeFactor;
	float orbitFactor;
	float rotateFactor;


	vector<float> planetsScale;
	vector<float> planetsRotationSpeed;
	vector<float> planetsOrbitSpeed;
	vector<float> planetsOffset;
	vector<vector<string>> planetsTexturePaths;
	vector<string> planetsObjectPaths;


	vector<float> moonsPlanetsScale;
	vector<float> moonsPlanetsRotationSpeed;
	vector<float> moonsPlanetsOrbitSpeed;
	vector<float> moonsPlanetsOffset;
	vector<float> indexMoon_Planets;
	vector<string> moonsPlanetsTexturePaths;
	vector<int> numOfMoons;
private:
	ConfigureLoader();
	~ConfigureLoader(){delete m_instance;}
	static ConfigureLoader * m_instance;
	string infile;
};


enum Planet{
	MERCURY,
	VENUS,
	EARTH,
	MARS,
	JUPITER,
	SATURN,
	URANUS,
	NEPTUNE,
	PLUTO
};



#endif /* INCLUDE_CONFIGURELOADER_H_ */
