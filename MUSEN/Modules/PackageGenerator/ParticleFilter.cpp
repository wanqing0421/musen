/* Copyright (c) 2013-2020, MUSEN Development Team. All rights reserved.
   This file is part of MUSEN framework http://msolids.net/musen.
   See LICENSE file for license and warranty information. */

#include "ParticleFilter.h"

CParticleFilter::CParticleFilter(CSystemStructure& _systemStructure, const SPackage& _generator) :
	m_systemStructure{ _systemStructure },
	m_generator{ _generator }
{
	// get generation volume
	const CAnalysisVolume* volume = _systemStructure.GetAnalysisVolume(m_generator.volumeKey);

	// setup volume checker to verify objects are inside the generation volume
	m_volumeChecker = new CInsideVolumeChecker{ volume, 0 };

	// setup volume checkers to verify that new objects are not situated in any real volume
	if (!_generator.insideGeometry)
		for (size_t i = 0; i < _systemStructure.GetGeometriesNumber(); ++i)
		{
			std::vector<STriangleType> triangles;
			for (const auto& wall : _systemStructure.GetGeometryWalls(i))
				triangles.push_back(wall->GetCoords(0));
			m_geometryCheckers.push_back(new CInsideVolumeChecker{ triangles, CVector3{0} });
		}
}

CParticleFilter::~CParticleFilter()
{
	delete m_volumeChecker;
	for (auto& checker : m_geometryCheckers)
		delete checker;
}

std::vector<size_t> CParticleFilter::Filter(const std::vector<CVector3>& _coords, const std::vector<double>& _radii)
{
	SPBC pbc = m_systemStructure.GetPBC();

	// get particles, which are inside the generation volume
	std::vector<size_t> validIDs = m_volumeChecker->GetSpheresTotallyInside(_coords, _radii);

	// filter out particles that are inside real geometries
	if (!m_generator.insideGeometry)
		for (const auto& checker : m_geometryCheckers)
		{
			std::vector<size_t> insideIDs = checker->GetSpheresPartiallyInside(_coords, _radii);
			validIDs = VectorDifference(validIDs, insideIDs);
		}

	// filter out particles that are outside PBC
	if (pbc.bEnabled)
	{
		// mark particles to delete
		std::transform(validIDs.begin(), validIDs.end(), validIDs.begin(), [&](const size_t &i) { return pbc.IsCoordInPBC(_coords[i], 0) ? i : size_t(-1); });
		// delete marked particles, without preserving order
		validIDs.erase(std::partition(validIDs.begin(), validIDs.end(), [&](const size_t &i) { return i != size_t(-1); }), validIDs.end());
	}

	// filter out particles that have contact with already existing ones
	const std::vector<size_t> existingParticles = m_systemStructure.GetParticleIndicesInVolume(0, m_generator.volumeKey, false);	// existing particles
	for (auto id : existingParticles)
	{
		const auto existPart = dynamic_cast<CSphere*>(m_systemStructure.GetObjectByIndex(id));
		const CVector3 ovrPartCoord = existPart->GetCoordinates(0);
		const double ovrPartRad = existPart->GetContactRadius();

		// mark particles to delete
		std::transform(validIDs.begin(), validIDs.end(), validIDs.begin(), [&](const size_t &i) { return Length(_coords[i], ovrPartCoord) >= _radii[i] + ovrPartRad ? i : size_t(-1); });
		// delete marked particles, without preserving order
		validIDs.erase(std::partition(validIDs.begin(), validIDs.end(), [&](const size_t &i) { return i != size_t(-1); }), validIDs.end());
	}

	return validIDs;
}
