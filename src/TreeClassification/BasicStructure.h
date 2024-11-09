

#ifndef _BASICSTRUCTURES_H_
#define _BASICSTRUCTURES_H_

#pragma once

#include<math.h>
#include<vector>
#include<limits.h>


#define PI	acos(-1)
#define INVALIDATE_VALUE	-99999999


//Structure definition of a float 3D point format;
struct fPoint3D
{
	float x;
	float y;
	float z;
	unsigned int state;
	fPoint3D()
	{
		x = y = z = 0.0;
		state = 0;
	}
	fPoint3D(fPoint3D* point)
	{
		x = point->x;
		y = point->y;
		z = point->z;
		state = point->state;
	}
	fPoint3D& operator=(fPoint3D* point)
	{
		this->x = point->x;
		this->y = point->y;
		this->z = point->z;
		this->state = point->state;
		return *this;
	}
};


//Structure definition of a 3D point in double;
struct dPoint3D
{
	double x;
	double y;
	double z;
	unsigned int id;
	unsigned int state;
	unsigned int neigh_count;
	int class_id;
	
	bool  isVisited;
	bool  isPushed;
	dPoint3D()
	{
		x = y = z = 0.0;
		state = 0;
		neigh_count = 0;
		class_id = -1;
		isVisited = false;
		isPushed = false;
		id = 0;
	}
	dPoint3D(const dPoint3D* point)
	{
		x = point->x;
		y = point->y;
		z = point->z;
		state = point->state;
		neigh_count = point->neigh_count;
		class_id = point->class_id;
		isVisited = point->isVisited;
		isPushed = point->isPushed;
		id = point->id;
	}
	dPoint3D& operator=(const dPoint3D* point)
	{
		this->x = point->x;
		this->y = point->y;
		this->z = point->z;
		this->state = point->state;
		this->neigh_count = point->neigh_count;
		this->class_id = point->class_id;
		this->isVisited = point->isVisited;
		this->isPushed = point->isPushed;
		this->id = point->id;

		return *this;
	}
};


//3D offset;
struct Offset
{
	double xOffset;
	double yOffset;
	double zOffset;
	Offset()
	{
		xOffset = yOffset = zOffset = 0.0;
	}
	Offset(const Offset* offset)
	{
		xOffset = offset->xOffset;
		yOffset = offset->yOffset;
		zOffset = offset->zOffset;
	}
	Offset& operator=(const Offset* offset)
	{
		xOffset = offset->xOffset;
		yOffset = offset->yOffset;
		zOffset = offset->zOffset;
		return *this;
	}
};




//RGB color definition;
struct PointColor
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	PointColor()
	{
		r = g = b = 0;
	}
	PointColor(const PointColor* pointcolor)
	{
		r = pointcolor->r;
		g = pointcolor->g;
		b = pointcolor->b;
	}
	PointColor& operator=(const PointColor* ptColor)
	{
		this->r = ptColor->r;
		this->g = ptColor->g;
		this->b = ptColor->b;
		return *this;
	}
};



//las points color definition;
struct PtsColor
{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	PtsColor()
	{
		red = green = blue;
	}
	PtsColor(PtsColor* lasptscolor)
	{
		red = lasptscolor->red;
		green = lasptscolor->green;
		blue = lasptscolor->blue;
	}
	PtsColor& operator=(const PtsColor* color)
	{
		this->red = color->red;
		this->green = color->green;
		this->blue = color->blue;

		return *this;
	}
};



//3D bounding box;
struct BoundBox
{
	double xMax;
	double yMax;
	double zMax;
	double xMin;
	double yMin;
	double zMin;
	BoundBox()
	{
		xMax = yMax = zMax = (std::numeric_limits<double>::min)();
		xMin = yMin = zMin = (std::numeric_limits<double>::max)();
	}
	BoundBox(const BoundBox* box)
	{
		xMax = box->xMax;
		xMin = box->xMin;
		yMax = box->yMax;
		yMin = box->yMin;
		zMax = box->zMax;
		zMin = box->xMin;
	}
	BoundBox& operator=(const BoundBox* box)
	{
		this->xMax = box->xMax;
		this->yMax = box->yMax;
		this->zMax = box->zMax;
		this->xMin = box->xMin;
		this->yMin = box->yMin;
		this->zMin = box->zMin;

		return *this;
	}
};


//header for 3D point cloud dataset
struct Point3DHeader
{
	long		PointsNumber;
	BoundBox	PointsBox;
	Offset		PointsOffset;
	char		FileInfo[32];
	Point3DHeader()
	{
		PointsNumber = 0;
		memset(FileInfo, 0, 32);
	}
};

//points definition used in Octree construction;
struct OctPt
{
	double x, y, z;
	OctPt& operator=(const OctPt& point)
	{
		x = point.x;
		y = point.y;
		z = point.z;
		return *this;
	}
	OctPt operator+(const OctPt& point)const
	{
		OctPt temp;
		temp.x = x + point.x;
		temp.y = y + point.y;
		temp.z = z + point.z;
		return temp;
	}
	OctPt operator-(const OctPt& point)const
	{
		OctPt temp;
		temp.x = x - point.x;
		temp.y = y - point.y;
		temp.z = z - point.z;
		return temp;
	}
	OctPt operator*(const double data)const
	{
		OctPt temp;
		temp.x = x * data;
		temp.y = y * data;
		temp.z = z * data;
		return temp;
	}
};




//For the alignment of the bits;
#pragma pack(push,BasicStructures,1)

//
//Las version 1.3 Format;
//--------------------------
//Public Header Block;
struct LasPublicHeader
{
	char			FileSignature[4];
	unsigned short	FileSourceID;//flight line number if this file was derived from an original flight line, [1,65535], 0 if not assigned;
	unsigned short	GlobalEncoding;//bit field encoding; [1,3] have special values;//[4,15] means reserved;
	unsigned long	GUIDData1;//Global Unique Identifier. 
	unsigned short	GUIDData2;
	unsigned short	GUIDData3;
	unsigned char	GUIDData4[8];
	unsigned char	VersionMajor;
	unsigned char	VersionMinor;
	char			SyetemIdentifier[32];
	char			GeneratingSoftware[32];
	unsigned short	FileCreationJulianDay;
	unsigned short	FileCreationYear;
	unsigned short	HeaderSize;
	unsigned long	OffsetToData;//offset to data;
	unsigned long	NumberofVaribleLengthRecords;
	unsigned char	PointDataFormatID;//points format identifier;
	unsigned short	PointDataRecordLength;
	unsigned long	NumberOfPoints;//points number;
	unsigned long	NumberOfPointsByReturn[5];
	double			xScale;
	double			yScale;
	double			zScale;
	double			xOffset;
	double			yOffset;
	double			zOffset;
	double			xMax;
	double			xMin;
	double			yMax;
	double			yMin;
	double			zMax;
	double			zMin;
	unsigned long long	StartWaveformDataPacketRecord;
	//provide offset, in bytes, from the beginning of the LAS file to the first WFDPRH;

	LasPublicHeader()
	{
		FileSignature[0] = 'L';
		FileSignature[1] = 'A';
		FileSignature[2] = 'S';
		FileSignature[3] = 'F';
		FileSourceID = 0;
		GlobalEncoding = 15;
		GUIDData1 = GUIDData2 = GUIDData3 = 0;
		memset(GUIDData4, 0, 8);
		VersionMajor = 0;
		VersionMinor = 0;
		memset(SyetemIdentifier, 0, 32);
		memset(GeneratingSoftware, 0, 32);
		FileCreationJulianDay = 0;
		FileCreationYear = 0;
		HeaderSize = 0;
		OffsetToData = 0;
		NumberofVaribleLengthRecords = 0;
		PointDataFormatID = 0;
		PointDataRecordLength = 0;
		NumberOfPoints = 0;
		memset(NumberOfPointsByReturn, 0, 5);
		xScale = yScale = zScale = 1.0;
		xOffset = yOffset = zOffset = 0.0;
		xMax = xMin = yMax = yMin = zMax = zMin = 0.0;
		StartWaveformDataPacketRecord = 0;
	}
	LasPublicHeader(LasPublicHeader* header)
	{
		FileSignature[0] = header->FileSignature[0];
		FileSignature[1] = header->FileSignature[1];
		FileSignature[2] = header->FileSignature[2];
		FileSignature[3] = header->FileSignature[3];
		FileSourceID = header->FileSourceID;
		GlobalEncoding = header->GlobalEncoding;
		GUIDData1 = header->GUIDData1;
		GUIDData2 = header->GUIDData2;
		GUIDData3 = header->GUIDData3;
		memcpy(GUIDData4, header->GUIDData4, 8);
		VersionMajor = header->VersionMajor;
		VersionMinor = header->VersionMinor;
		memcpy(SyetemIdentifier, header->SyetemIdentifier, 32);
		memcpy(GeneratingSoftware, header->GeneratingSoftware, 32);
		FileCreationJulianDay = header->FileCreationJulianDay;
		FileCreationYear = header->FileCreationYear;
		HeaderSize = header->HeaderSize;
		OffsetToData = header->OffsetToData;
		NumberofVaribleLengthRecords = header->NumberofVaribleLengthRecords;
		PointDataFormatID = header->PointDataFormatID;
		PointDataRecordLength = header->PointDataRecordLength;
		NumberOfPoints = header->NumberOfPoints;
		memcpy(NumberOfPointsByReturn, header->NumberOfPointsByReturn, 5);
		xScale = header->xScale;
		yScale = header->yScale;
		zScale = header->zScale;
		xOffset = header->xOffset;
		yOffset = header->yOffset;
		zOffset = header->zOffset;
		xMax = header->xMax;
		xMin = header->xMin;
		yMax = header->yMax;
		yMin = header->yMin;
		zMax = header->zMax;
		zMin = header->zMin;
		StartWaveformDataPacketRecord = header->StartWaveformDataPacketRecord;
	}
};
//Variable Length Record Header


struct LasVariableLengthRecordHeader
{
	unsigned short		Reserved;
	char				UserID[16];
	unsigned short		RecordID;
	unsigned short		RecordLengthAfterHeader;
	char				Description[32];
	LasVariableLengthRecordHeader()
	{
		Reserved = 0;
		memset(UserID, 0, 16);
		RecordID = 0;
		RecordLengthAfterHeader = 0;
		memset(Description, 0, 32);
	}
	LasVariableLengthRecordHeader(LasVariableLengthRecordHeader* variableheader)
	{
		Reserved = variableheader->Reserved;
		memcpy(UserID, variableheader->UserID, 16);
		RecordID = variableheader->RecordID;
		RecordLengthAfterHeader = variableheader->RecordLengthAfterHeader;
		memcpy(Description, variableheader->Description, 32);
	}
};


struct LasPointDataRecordFormat0
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;

	LasPointDataRecordFormat0()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
	}
	LasPointDataRecordFormat0(LasPointDataRecordFormat0* lasformat0)
	{
		X = lasformat0->X;
		Y = lasformat0->Y;
		Z = lasformat0->Z;
		Intensity = lasformat0->Intensity;
		ReturnNumber = lasformat0->ReturnNumber;
		NumberOfReturns = lasformat0->NumberOfReturns;
		ScanDirectionFlag = lasformat0->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat0->EdgeOfFlightLine;
		Classification = lasformat0->Classification;
		ScanAngleRank = lasformat0->ScanAngleRank;
		UserData = lasformat0->UserData;
		PointSourceID = lasformat0->PointSourceID;
	}
};



//Point Data Record Format 1
struct LasPointDataRecordFormat1
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;
	double				GPSTime;

	LasPointDataRecordFormat1()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
		GPSTime = 0.0;
	}
	LasPointDataRecordFormat1(LasPointDataRecordFormat1* lasformat1)
	{
		X = lasformat1->X;
		Y = lasformat1->Y;
		Z = lasformat1->Z;
		Intensity = lasformat1->Intensity;
		ReturnNumber = lasformat1->ReturnNumber;
		NumberOfReturns = lasformat1->NumberOfReturns;
		ScanDirectionFlag = lasformat1->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat1->EdgeOfFlightLine;
		Classification = lasformat1->Classification;
		ScanAngleRank = lasformat1->ScanAngleRank;
		UserData = lasformat1->UserData;
		PointSourceID = lasformat1->PointSourceID;
		GPSTime = lasformat1->GPSTime;
	}
};



//Point Data Record Format 2
struct LasPointDataRecordFormat2
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;
	unsigned short		red;
	unsigned short		green;
	unsigned short		blue;

	LasPointDataRecordFormat2()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
		red = green = blue = 0;
	}
	LasPointDataRecordFormat2(LasPointDataRecordFormat2* lasformat2)
	{
		X = lasformat2->X;
		Y = lasformat2->Y;
		Z = lasformat2->Z;
		Intensity = lasformat2->Intensity;
		ReturnNumber = lasformat2->ReturnNumber;
		NumberOfReturns = lasformat2->NumberOfReturns;
		ScanDirectionFlag = lasformat2->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat2->EdgeOfFlightLine;
		Classification = lasformat2->Classification;
		ScanAngleRank = lasformat2->ScanAngleRank;
		UserData = lasformat2->UserData;
		PointSourceID = lasformat2->PointSourceID;
		red = lasformat2->red;
		green = lasformat2->green;
		blue = lasformat2->blue;
	}
};

//Point Data Record Format 3
struct LasPointDataRecordFormat3
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;
	double				GPSTime;
	unsigned short		red;
	unsigned short		green;
	unsigned short		blue;

	LasPointDataRecordFormat3()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
		GPSTime = 0.0;
		red = green = blue = 0;
	}
	LasPointDataRecordFormat3(LasPointDataRecordFormat3* lasformat3)
	{
		X = lasformat3->X;
		Y = lasformat3->Y;
		Z = lasformat3->Z;
		Intensity = lasformat3->Intensity;
		ReturnNumber = lasformat3->ReturnNumber;
		NumberOfReturns = lasformat3->NumberOfReturns;
		ScanDirectionFlag = lasformat3->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat3->EdgeOfFlightLine;
		Classification = lasformat3->Classification;
		ScanAngleRank = lasformat3->ScanAngleRank;
		UserData = lasformat3->UserData;
		PointSourceID = lasformat3->PointSourceID;
		GPSTime = lasformat3->GPSTime;
		red = lasformat3->red;
		green = lasformat3->green;
		blue = lasformat3->blue;
	}
};

//Point Data Record Format 4
struct LasPointDataRecordFormat4
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;
	double				GPSTime;
	unsigned char		WavePacketDescriptorIndex;//0 if has no waveform data;
	unsigned long long	ByteOffsetToWaveformData;//location of the start of this LiDAR WF packet;
	unsigned long		WaveformPacketSizeInBytes;//size of the WF packet;
	float				ReturnPointWaveformLocation;//the offset in picoseconds(10e-12) from the first digitized value to the location within t he WF packet
	float				Xt;//define a parametric line wquation for extrapolating points along the associated WF;
	float				Yt;//X = X0 +Xt same with Yt and Zt;
	float				Zt;

	LasPointDataRecordFormat4()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
		GPSTime = 0.0;
		WavePacketDescriptorIndex = 0;
		ByteOffsetToWaveformData = 0;
		WaveformPacketSizeInBytes = 0;
		ReturnPointWaveformLocation = 0;
		Xt = Yt = Zt = 0.0;
	}
	LasPointDataRecordFormat4(LasPointDataRecordFormat4* lasformat4)
	{
		X = lasformat4->X;
		Y = lasformat4->Y;
		Z = lasformat4->Z;
		Intensity = lasformat4->Intensity;
		ReturnNumber = lasformat4->ReturnNumber;
		NumberOfReturns = lasformat4->NumberOfReturns;
		ScanDirectionFlag = lasformat4->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat4->EdgeOfFlightLine;
		Classification = lasformat4->Classification;
		ScanAngleRank = lasformat4->ScanAngleRank;
		UserData = lasformat4->UserData;
		PointSourceID = lasformat4->PointSourceID;
		GPSTime = lasformat4->GPSTime;
		WavePacketDescriptorIndex = lasformat4->WavePacketDescriptorIndex;
		ByteOffsetToWaveformData = lasformat4->ByteOffsetToWaveformData;
		WaveformPacketSizeInBytes = lasformat4->WaveformPacketSizeInBytes;
		ReturnPointWaveformLocation = lasformat4->ReturnPointWaveformLocation;
		Xt = lasformat4->Xt;
		Yt = lasformat4->Yt;
		Zt = lasformat4->Zt;
	}
};

//Point Data Record Format 5
struct LasPointDataRecordFormat5
{
	long				X;
	long				Y;
	long				Z;
	unsigned short		Intensity;
	unsigned char		ReturnNumber : 3;
	unsigned char		NumberOfReturns : 3;
	unsigned char		ScanDirectionFlag : 1;
	unsigned char		EdgeOfFlightLine : 1;
	unsigned char		Classification;
	char				ScanAngleRank;
	unsigned char		UserData;
	unsigned short		PointSourceID;
	double				GPSTime;
	unsigned short		red;
	unsigned short		green;
	unsigned short		blue;
	unsigned char		WavePacketDescriptorIndex;//0 if has no waveform data;
	unsigned long long	ByteOffsetToWaveformData;//location of the start of this LiDAR WF packet;
	unsigned long		WaveformPacketSizeInBytes;//size of the WF packet;
	float				ReturnPointWaveformLocation;//the offset in picoseconds(10e-12) from the first digitized value to the location within t he WF packet
	float				Xt;//define a parametric line equation for extrapolating points along the associated WF;
	float				Yt;
	float				Zt;

	LasPointDataRecordFormat5()
	{
		X = Y = Z = 0;
		Intensity = 0;
		ReturnNumber = 1;
		NumberOfReturns = 1;
		ScanDirectionFlag = 1;
		EdgeOfFlightLine = 0;
		Classification = 0;
		ScanAngleRank = 0;
		UserData = 0;
		PointSourceID = 0;
		GPSTime = 0.0;
		red = green = blue = 0;
		WavePacketDescriptorIndex = 0;
		ByteOffsetToWaveformData = 0;
		WaveformPacketSizeInBytes = 0;
		ReturnPointWaveformLocation = 0;
		Xt = Yt = Zt = 0.0;
	}
	LasPointDataRecordFormat5(LasPointDataRecordFormat5* lasformat5)
	{
		X = lasformat5->X;
		Y = lasformat5->Y;
		Z = lasformat5->Z;
		Intensity = lasformat5->Intensity;
		ReturnNumber = lasformat5->ReturnNumber;
		NumberOfReturns = lasformat5->NumberOfReturns;
		ScanDirectionFlag = lasformat5->ScanDirectionFlag;
		EdgeOfFlightLine = lasformat5->EdgeOfFlightLine;
		Classification = lasformat5->Classification;
		ScanAngleRank = lasformat5->ScanAngleRank;
		UserData = lasformat5->UserData;
		PointSourceID = lasformat5->PointSourceID;
		GPSTime = lasformat5->GPSTime;
		red = lasformat5->red;
		green = lasformat5->green;
		blue = lasformat5->blue;
		WaveformPacketSizeInBytes = lasformat5->WaveformPacketSizeInBytes;
		ByteOffsetToWaveformData = lasformat5->ByteOffsetToWaveformData;
		WaveformPacketSizeInBytes = lasformat5->WaveformPacketSizeInBytes;
		ReturnPointWaveformLocation = lasformat5->ReturnPointWaveformLocation;
		Xt = lasformat5->Xt;
		Yt = lasformat5->Yt;
		Zt = lasformat5->Zt;
	}
};


struct WaveformPacketDescriptorUserDefinedRecord
{
	unsigned char		BitsPerSample;
	unsigned char		WaveformCompressionType;//0 indicates there is no compression;
	unsigned long		NumberOfSamples;//represents the fully decompressed waveform packet
	unsigned long		TemporalSampleSpacing;//temporal sample spacing in picoseconds(10e-12 second);
	double				DigitizerGain;//the gain and off#set are used to convert the raw digitized value to an absolute digitizer voltage using
	double				DigitizerOffset;//->Volts = offset + gain*raw_waveform_amplitude
	WaveformPacketDescriptorUserDefinedRecord()
	{
		BitsPerSample = 0;
		WaveformCompressionType = 0;
		NumberOfSamples = 0;
		TemporalSampleSpacing = 0;
		DigitizerGain = 0.0;
		DigitizerOffset = 0.0;
	}
	WaveformPacketDescriptorUserDefinedRecord(WaveformPacketDescriptorUserDefinedRecord* userdefindrecord)
	{
		BitsPerSample = userdefindrecord->BitsPerSample;
		WaveformCompressionType = userdefindrecord->WaveformCompressionType;
		NumberOfSamples = userdefindrecord->NumberOfSamples;
		TemporalSampleSpacing = userdefindrecord->TemporalSampleSpacing;
		DigitizerGain = userdefindrecord->DigitizerGain;
		DigitizerOffset = userdefindrecord->DigitizerOffset;
	}
};



struct ExtendedVariableLengthRecordHeader
{
	unsigned short			Reserved;
	char					UserID[16];
	unsigned short			RecordID;
	unsigned long long		RecordLengthAfterHeader;
	char					Description[32];

	ExtendedVariableLengthRecordHeader()
	{
		Reserved = 0;
		memset(UserID, 0, 16);
		RecordID = 0;
		RecordLengthAfterHeader = 0;
		memset(Description, 0, 32);
	}
	ExtendedVariableLengthRecordHeader(ExtendedVariableLengthRecordHeader* header)
	{
		Reserved = header->Reserved;
		memcpy(UserID, header->UserID, 16);
		RecordID = header->RecordID;
		RecordLengthAfterHeader = header->RecordLengthAfterHeader;
		memcpy(Description, header->Description, 32);
	}
};


struct WFTimeFlag
{
	float Xt;
	float Yt;
	float Zt;
	WFTimeFlag()
	{
		Xt = Yt = Zt = 0.0;
	}
	WFTimeFlag(WFTimeFlag* timeflag)
	{
		Xt = timeflag->Xt;
		Yt = timeflag->Yt;
		Zt = timeflag->Zt;
	}
};

#pragma pack(pop,BasicStructures)


#endif