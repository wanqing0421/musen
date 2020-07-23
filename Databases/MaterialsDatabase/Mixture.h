/* Copyright (c) 2013-2020, MUSEN Development Team. All rights reserved.
   This file is part of MUSEN framework http://msolids.net/musen.
   See LICENSE file for license and warranty information. */

#pragma once

#include "DefinesMDB.h"
#include "MUSENStringFunctions.h"
#pragma warning(push, 0)
#include "GeneratedFiles/MaterialsDatabase.pb.h"
#pragma warning(pop)

struct SCompoundFraction
{
	std::string sFractionName;
	std::string sCompoundKey;
	double dDiameter;
	double dContactDiameter;
	double dFraction;
	SCompoundFraction():
		sFractionName(COMPOUND_FRACTION_UNDEFINED_NAME), sCompoundKey(""), dDiameter(COMPOUND_FRACTION_DEFAULT_DIAMETER), dContactDiameter(COMPOUND_FRACTION_DEFAULT_DIAMETER), dFraction(0) {}
	SCompoundFraction(const std::string& _sName, const std::string& _sCompoundKey, double _dDiameter, double _dFraction) :
		sFractionName(_sName), sCompoundKey(_sCompoundKey), dDiameter(_dDiameter), dContactDiameter(_dDiameter), dFraction(_dFraction) {}
	SCompoundFraction(const std::string& _sName, const std::string& _sCompoundKey, double _dDiameter, double _dContactDiameter, double _dFraction) :
		sFractionName(_sName), sCompoundKey(_sCompoundKey), dDiameter(_dDiameter), dContactDiameter(_dContactDiameter), dFraction(_dFraction) {}
	/// Saves CompoundFraction to protobuf-file
	void SaveToProtobuf(ProtoCompoundFraction& _protoFraction)
	{
		_protoFraction.set_fractionname(sFractionName);
		_protoFraction.set_compoundkey(sCompoundKey);
		_protoFraction.set_diameter(dDiameter);
		_protoFraction.set_contact_diameter(dContactDiameter);
		_protoFraction.set_fraction(dFraction);
	}
	/// Loads CompoundFraction from protobuf-file
	void LoadFromProtobuf(const ProtoCompoundFraction& _protoFraction)
	{
		sFractionName = _protoFraction.fractionname();
		sCompoundKey = _protoFraction.compoundkey();
		dDiameter = _protoFraction.diameter();
		if (_protoFraction.contact_diameter()!=0) // for old file formats //proto3 (in proto2 - has_contact_diameter)
			dContactDiameter = _protoFraction.contact_diameter();
		else
			dContactDiameter = dDiameter;
		dFraction = _protoFraction.fraction();
	}
};

class CMixture
{
private:
	std::string m_sUniqueKey;	/// Key to identify this mixture.
	std::string m_sMixtureName; /// Name of the mixture.
	std::vector<SCompoundFraction*> m_vFractions;

public:
	CMixture(const std::string& _sKey = "");
	CMixture(const std::string& _sKey, const std::string& _sName);
	CMixture(const CMixture& _mixture);
	~CMixture();

	//////////////////////////////////////////////////////////////////////////
	/// Functions to work with main info

	/// Returns name of the mixture.
	std::string GetName() const;
	/// Sets new name of the mixture.
	void SetName(const std::string& _sName);
	/// Returns unique key of the mixture.
	std::string GetKey() const;
	/// Sets new unique key of the mixture.
	void SetKey(const std::string& _sKey);
	/// Removes all fractions from the mixture.
	void Clear();

	//////////////////////////////////////////////////////////////////////////
	/// Functions to work with fractions

	/// Returns number of defined fractions.
	size_t FractionsNumber() const;
	/// Adds new empty fraction to the mixture. Returns index of the fraction.
	size_t AddFraction();
	/// Adds new fraction to the mixture with specified parameters. Contact diameter is set equal to diameter. Returns index of the fraction.
	size_t AddFraction(const std::string& _sName, const std::string& _sCompoundKey, double _dDiameter, double _dFraction);
	/// Adds new fraction to the mixture with specified parameters. Returns index of the fraction.
	size_t AddFraction(const std::string& _sName, const std::string& _sCompoundKey, double _dDiameter, double _dContactDiameter, double _dFraction);
	/// Sets new name of the specified fraction.
	void SetFractionName(size_t _iFraction, const std::string& _sName);
	/// Sets new compound of the specified fraction.
	void SetFractionCompound(size_t _iFraction, const std::string& _sCompoundKey);
	/// Sets new diameter of the specified fraction.
	void SetFractionDiameter(size_t _iFraction, double _dDiameter);
	/// Sets new contact diameter of the specified fraction.
	void SetFractionContactDiameter(size_t _iFraction, double _dContactDiameter);
	/// Sets new value of the specified fraction.
	void SetFractionValue(size_t _iFraction, double _dFraction);
	/// Returns name of the specified fraction. Returns empty string if such fraction has not been defined.
	std::string GetFractionName(size_t _iFraction) const;
	/// Returns compound key of the specified fraction. Returns empty string if such fraction has not been defined.
	std::string GetFractionCompound(size_t _iFraction) const;
	/// Returns diameter of the specified fraction. Returns NaN if such fraction has not been defined.
	double GetFractionDiameter(size_t _iFraction) const;
	/// Returns contact diameter of the specified fraction. Returns NaN if such fraction has not been defined.
	double GetFractionContactDiameter(size_t _iFraction) const;
	/// Returns minimum diameter of particles within the mixture.
	double GetMinFractionDiameter() const;
	/// Returns maximum diameter of particles within the mixture.
	double GetMaxFractionDiameter() const;
	/// Returns minimum contact diameter of particles within the mixture.
	double GetMinFractionContactDiameter() const;
	/// Returns maximum contact diameter of particles within the mixture.
	double GetMaxFractionContactDiameter() const;
	/// Returns value of the specified fraction. Returns 0 if such fraction has not been defined.
	double GetFractionValue(size_t _iFraction) const;
	/// Removes specified fraction from the mixture.
	void RemoveFraction(size_t _iFraction);
	/// Normalizes fractions in mixture.
	void NormalizeFractions();
	/// Moves selected fraction upwards in the list of compounds.
	void UpFraction(size_t _iFraction);
	/// Moves selected fraction downwards in the list of compounds.
	void DownFraction(size_t _iFraction);

	//////////////////////////////////////////////////////////////////////////
	/// Save/Load

	/// Saves Mixture to protobuf file.
	void SaveToProtobuf(ProtoMixture& _protoMixture);
	/// Loads Mixture from protobuf file.
	void LoadFromProtobuf(const ProtoMixture& _protoMixture);
};

