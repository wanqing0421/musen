#pragma once

#include "AbstractDEMModel.h"

class CModelPPSinteringTemperature : public CParticleParticleModel
{
public:
	CModelPPSinteringTemperature();
	void CalculatePPForce(double _time, double _timeStep, size_t _iSrc, size_t _iDst, const SInteractProps& _interactProp, SCollision* _collision) const override;
	void ConsolidateSrc(double _time, double _timeStep, size_t _iPart, SParticleStruct& _particles, const SCollision* _collision) const override;
	void ConsolidateDst(double _time, double _timeStep, size_t _iPart, SParticleStruct& _particles, const SCollision* _collision) const override;
	void SetParametersGPU(const std::vector<double>& _parameters, const SPBC& _pbc) override;
	void CalculatePPForceGPU(double _time, double _timeStep, const SInteractProps _interactProps[],	const SGPUParticles& _particles, SGPUCollisions& _collisions) override;
};