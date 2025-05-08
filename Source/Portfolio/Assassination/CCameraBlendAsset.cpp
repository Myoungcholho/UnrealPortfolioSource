#include "../Assassination/CCameraBlendAsset.h"
#include "../Global.h"

const FCameraBlendData* UCCameraBlendAsset::FindBlendDataByName(FName InName) const
{
	for (const FCameraBlendData& Data : BlendDatas)
	{
		if (Data.BlendName == InName)
		{
			return &Data;
		}
	}

	return nullptr;
}
