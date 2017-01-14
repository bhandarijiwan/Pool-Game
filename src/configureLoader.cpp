/*
 * configureLoader.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: check
 */

#include <configureLoader.h>
#include <iostream>

ConfigureLoader * ConfigureLoader::m_instance = NULL;

ConfigureLoader::ConfigureLoader()
{
	infile = "../data/configureFile";
	ifstream ifs(infile);
	string str = "";
	float f;
	while(true)
	{
		ifs>>str;
		if(str == "EOF")
			break;
		ifs>>f;
		if(str == "SunScale")
			sunScale =f;

		else if(str == "MercuryScale")
			mercuryScale = f;
		else if(str == "MercuryOrbitPeriod")
			mercuryOrbitPeriod = f;
		else if(str == "MercuryRotationPeriod")
			mercuryRotationPeriod = f;
		else if(str == "MercuryOffset")
			mercuryOffset = f;

		else if(str == "VenusScale")
			venusScale = f;
		else if(str == "VenusOrbitPeriod")
			venusOrbitPeriod = f;
		else if(str == "VenusRotationPeriod")
			venusRotationPeriod = f;
		else if(str == "VenusOffset")
			venusOffset = f;

		else if(str == "EarthScale")
			earthScale = f;
		else if(str == "EarthOrbitPeriod")
			earthOrbitPeriod = f;
		else if(str == "EarthRotationPeriod")
			earthRotationPeriod = f;
		else if(str == "EarthOffset")
			earthOffset = f;
		else if(str == "EarthMajorOfOrbit")
			earthMajorOfOrbit = f;
		else if(str == "EarthRatioOfMinorToMajor")
			earthRatioOfMinorToMajor = f;
		else if(str == "MoonOfPlanetIndex0")
			moonOfPlanetIndex0 = (int) f;

		else if(str == "MoonScale")
		{
			moonScale = f;
			moonsPlanetsScale.push_back(moonScale);
		}
		else if(str == "MoonOrbitPeriod")
		{
			moonOrbitPeriod = f;
			moonsPlanetsOrbitSpeed.push_back(moonOrbitPeriod);
		}
		else if(str == "MoonRotationPeriod")
		{
			moonRotationPeriod = f;
			moonsPlanetsRotationSpeed.push_back(moonRotationPeriod);
		}
		else if(str == "MoonOffset")
		{
			moonOffset = f;
			moonsPlanetsOffset.push_back(moonOffset);
		}

		else if(str == "MarsScale")
			marsScale = f;
		else if(str == "MarsOrbitPeriod")
			marsOrbitPeriod = f;
		else if(str == "MarsRotationPeriod")
			marsRotationPeriod = f;
		else if(str == "MarsOffset")
			marsOffset = f;

		else if(str == "JupiterScale")
			jupiterScale = f;
		else if(str == "JupiterOrbitPeriod")
			jupiterOrbitPeriod = f;
		else if(str == "JupiterRotationPeriod")
			jupiterRotationPeriod = f;
		else if(str == "JupiterOffset")
			jupiterOffset = f;

		else if(str == "SaturnScale")
			saturnScale = f;
		else if(str == "SaturnOrbitPeriod")
			saturnOrbitPeriod = f;
		else if(str == "SaturnRotationPeriod")
			saturnRotationPeriod = f;
		else if(str == "SaturnOffset")
			saturnOffset = f;

		else if(str == "UranusScale")
			uranusScale = f;
		else if(str == "UranusOrbitPeriod")
			uranusOrbitPeriod = f;
		else if(str == "UranusRotationPeriod")
			uranusRotationPeriod = f;
		else if(str == "UranusOffset")
			uranusOffset = f;

		else if(str == "NeptuneScale")
			neptuneScale = f;
		else if(str == "NeptuneOrbitPeriod")
			neptuneOrbitPeriod = f;
		else if(str == "NeptuneRotationPeriod")
			neptuneRotationPeriod = f;
		else if(str == "NeptuneOffset")
			neptuneOffset = f;

		else if(str == "PlutoScale")
			plutoScale = f;
		else if(str == "PlutoOrbitPeriod")
			plutoOrbitPeriod = f;
		else if(str == "PlutoRotationPeriod")
			plutoRotationPeriod = f;
		else if(str == "PlutoOffset")
			plutoOffset = f;


		else if(str == "DistanceFactor")
			distanceFactor = f;
		else if(str == "SizeFactor")
			sizeFactor = f;
		else if(str == "OrbitFactor")
			orbitFactor = f;
		else if(str == "RotateFactor")
			rotateFactor = f;
		else if(str == "NumOfMoons")
		{
			numOfMoons.push_back((int)f);

		}
	}
	ifs.close();

	planetsScale.push_back(mercuryScale);
	planetsScale.push_back(venusScale);
	planetsScale.push_back(earthScale);
	planetsScale.push_back(marsScale);
	planetsScale.push_back(jupiterScale);
	planetsScale.push_back(saturnScale);
	planetsScale.push_back(uranusScale);
	planetsScale.push_back(neptuneScale);
	planetsScale.push_back(plutoScale);

	planetsRotationSpeed.push_back(mercuryRotationPeriod);
	planetsRotationSpeed.push_back(venusRotationPeriod);
	planetsRotationSpeed.push_back(earthRotationPeriod);
	planetsRotationSpeed.push_back(marsRotationPeriod);
	planetsRotationSpeed.push_back(jupiterRotationPeriod);
	planetsRotationSpeed.push_back(saturnRotationPeriod);
	planetsRotationSpeed.push_back(uranusRotationPeriod);
	planetsRotationSpeed.push_back(neptuneRotationPeriod);
	planetsRotationSpeed.push_back(plutoRotationPeriod);

	planetsOrbitSpeed.push_back(mercuryOrbitPeriod);
	planetsOrbitSpeed.push_back(venusOrbitPeriod);
	planetsOrbitSpeed.push_back(earthOrbitPeriod);
	planetsOrbitSpeed.push_back(marsOrbitPeriod);
	planetsOrbitSpeed.push_back(jupiterOrbitPeriod);
	planetsOrbitSpeed.push_back(saturnOrbitPeriod);
	planetsOrbitSpeed.push_back(uranusOrbitPeriod);
	planetsOrbitSpeed.push_back(neptuneOrbitPeriod);
	planetsOrbitSpeed.push_back(plutoOrbitPeriod);

	planetsOffset.push_back(mercuryOffset);
	planetsOffset.push_back(venusOffset);
	planetsOffset.push_back(earthOffset);
	planetsOffset.push_back(marsOffset);
	planetsOffset.push_back(jupiterOffset);
	planetsOffset.push_back(saturnOffset);
	planetsOffset.push_back(uranusOffset);
	planetsOffset.push_back(neptuneOffset);
	planetsOffset.push_back(plutoOffset);

	planetsTexturePaths.push_back({"../data/textures/texture_mercury.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_venus.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_earth.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_mars.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_jupiter.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_saturn.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_uranus.jpg"});
    planetsTexturePaths.push_back({"../data/textures/texture_neptune.jpg"});
	planetsTexturePaths.push_back({"../data/textures/texture_pluto.jpg"});


	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/saturn.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");
	planetsObjectPaths.push_back("../data/sun.obj");


	moonsPlanetsTexturePaths.push_back("../data/textures/texture_moon.jpg");
	indexMoon_Planets.push_back(moonOfPlanetIndex0);
}
//singleton
ConfigureLoader * ConfigureLoader::GetInstance()
{
	if(!m_instance)
	{
		m_instance = new ConfigureLoader();
	}
	return m_instance;
}
