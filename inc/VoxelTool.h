#ifndef _VoxelTool_
#define _VoxelTool_
#include <AIS_Shape.hxx>
#include <Voxel_Prs.hxx>
#include <Voxel_BoolDS.hxx>
#include <Voxel_ColorDS.hxx>
#include <Voxel_CollisionDetection.hxx>
#include <Voxel_FastConverter.hxx>

#include <map>
#include <vector>
class Assembly;
class Part;
class AsmTreeNode;

class VoxelTool: public Voxel_CollisionDetection{
public :
	VoxelTool(){}

//!Init Voxel representation from Assembly structure
	Standard_Integer Init(Assembly *product);

	Standard_Integer Free();
//!Create or update voxel representation after adding or erasing parts
	Standard_Integer UpdateVoxelModel();
//! Add part to Voxel Manager
//! ifUpdateVoxelStructure = true, after adding new part 
//!	UpdateVoxelModel procedure will be called.
	Standard_Integer AddPart(Part* part, Standard_Boolean updateVoxelStructure = false);

	Standard_Integer MovePart(Standard_Integer partUri, gp_Vec move);

	Standard_Integer HidePart(Standard_Integer partUri);
//! Return list of part's Uri for parts which blocked this assembly operation
	std::vector<Standard_Integer> &&CheckOperation(const AsmTreeNode& asmOperation);

	Standard_Integer ErasePart(Standard_Integer partUri);

	Standard_Integer CountAllCollision();

	Standard_Integer CountClsnForPart(Standard_Integer partUri);



	~VoxelTool();

protected:
   

	std::map<Standard_Integer, Part*> partMap;
	std::map<Standard_Integer, Voxel_BoolDS*> voxelMap;

	//Handle(Voxel_Prs) myVoxels;

	Standard_Integer myNbX;
	Standard_Integer myNbY;
	Standard_Integer myNbZ;
	Standard_Real xLen;
	Standard_Real yLen;
	Standard_Real zLen;


};

#endif