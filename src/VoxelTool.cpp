#include "VoxelTool.h"

//!Init Voxel representation from Assembly structure
Standard_Integer VoxelTool::Init(Assembly *product){
	return 0;
}

Standard_Integer VoxelTool::Free(){
	return 0;
}
//!Create or update voxel representation after adding or erasing parts
Standard_Integer VoxelTool::UpdateVoxelModel(){
	return 0;
}
//! Add part to Voxel Manager
//! ifUpdateVoxelStructure = true, after adding new part 
//!	UpdateVoxelModel procedure will be called.
Standard_Integer VoxelTool::AddPart(Part* part, Standard_Boolean isUpdate){
	return 0;
}

Standard_Integer VoxelTool::MovePart(Standard_Integer partUri, gp_Vec move){
	return 0;
}

Standard_Integer VoxelTool::HidePart(Standard_Integer partUri){
	return 0;
}

Standard_Integer VoxelTool::ErasePart(Standard_Integer partUri){
	return 0;
}

Standard_Integer VoxelTool::CountAllCollision(){
	return 0;
}

Standard_Integer VoxelTool::CountClsnForPart(Standard_Integer partUri){
	return 0;
}

VoxelTool::~VoxelTool(){
	
}

