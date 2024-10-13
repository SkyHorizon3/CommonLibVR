#pragma once

#include "RE/B/BSTArray.h"
#include "RE/N/NiCamera.h"
#include "RE/N/NiSmartPointer.h"
#include "RE/N/NiSourceTexture.h"
#include "SKSE/Version.h"

namespace RE
{
	namespace BSGraphics
	{
		//WARNING: Structs containing ViewData appear to break when returned via RelocateMember due to incorrect offsets.
		struct alignas(16) ViewData
		{
			std::array<uint32_t, 4>  viewUp;                            // 00
			std::array<uint32_t, 4>  viewRight;                         // 10
			std::array<uint32_t, 4>  viewForward;                       // 20
			std::array<uint32_t, 16> viewMat;                           // 30
			std::array<uint32_t, 16> projMat;                           // 70
			std::array<uint32_t, 16> viewProjMat;                       // B0
			std::array<uint32_t, 16> unknownMat1;                       // F0 - all 0?
			std::array<uint32_t, 16> viewProjMatrixUnjittered;          // 130
			std::array<uint32_t, 16> previousViewProjMatrixUnjittered;  // 170
			std::array<uint32_t, 16> projMatrixUnjittered;              // 1B0
			std::array<uint32_t, 16> unknownMat2;                       // 1F0 - all 0?
			float                    viewPort[4];                       // 230 - NiRect<float> { left = 0, right = 1, top = 1, bottom = 0 }
			NiPoint2                 viewDepthRange;                    // 240
			char                     _pad0[0x8];                        // 248
		};
		static_assert(sizeof(ViewData) == 0x250);
		static_assert(offsetof(ViewData, viewUp) == 0);
		static_assert(offsetof(ViewData, viewRight) == 0x10);
		static_assert(offsetof(ViewData, viewForward) == 0x20);
		static_assert(offsetof(ViewData, viewMat) == 0x30);
		static_assert(offsetof(ViewData, projMat) == 0x70);
		static_assert(offsetof(ViewData, viewProjMat) == 0xb0);
		static_assert(offsetof(ViewData, unknownMat1) == 0xf0);
		static_assert(offsetof(ViewData, viewProjMatrixUnjittered) == 0x130);
		static_assert(offsetof(ViewData, previousViewProjMatrixUnjittered) == 0x170);
		static_assert(offsetof(ViewData, projMatrixUnjittered) == 0x1b0);
		static_assert(offsetof(ViewData, unknownMat2) == 0x1f0);
		static_assert(offsetof(ViewData, viewPort) == 0x230);
		static_assert(offsetof(ViewData, viewDepthRange) == 0x240);

		struct CAMERASTATE_RUNTIME_DATA
		{
#if !defined(ENABLE_SKYRIM_VR)  // Non-VR
#	define CAMERASTATE_RUNTIME_DATA_CONTENT                                                                       \
		ViewData camViewData;               /* 08 VR is BSTArray, Each array has 2 elements (one for each eye?) */ \
		NiPoint3 posAdjust;                 /* 20 */                                                               \
		NiPoint3 currentPosAdjust;          /* 38 */                                                               \
		NiPoint3 previousPosAdjust;         /* 50 */                                                               \
		#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
#	define CAMERASTATE_RUNTIME_DATA_CONTENT                                                                         \
		BSTArray<ViewData> camViewData;       /* 08 VR is BSTArray, Each array has 2 elements (one for each eye?) */ \
		BSTArray<NiPoint3> posAdjust;         /* 20 */                                                               \
		BSTArray<NiPoint3> currentPosAdjust;  /* 38 */                                                               \
		BSTArray<NiPoint3> previousPosAdjust; /* 50 */
#else
#	define CAMERASTATE_RUNTIME_DATA_CONTENT
#endif
			//members
			CAMERASTATE_RUNTIME_DATA_CONTENT;  // 08
			bool     useJitter;                /* 68 */
			uint32_t numData;                  /* 6c */
		};

		class CameraStateData
		{
		public:
			[[nodiscard]] CAMERASTATE_RUNTIME_DATA& GetCameraStateRuntimeData() noexcept
			{
				return REL::RelocateMember<CAMERASTATE_RUNTIME_DATA>(this, 0x8, 0x8);
			}

			[[nodiscard]] inline const CAMERASTATE_RUNTIME_DATA& GetCameraStateRuntimeData() const noexcept
			{
				return REL::RelocateMember<CAMERASTATE_RUNTIME_DATA>(this, 0x8, 0x8);
			}

			// members
			NiCamera* referenceCamera;         /* 00 */
			CAMERASTATE_RUNTIME_DATA_CONTENT;  // 08
		};
#if !defined(ENABLE_SKYRIM_VR)  // Non-VR
		static_assert(sizeof(CameraStateData) == 0x68);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
		static_assert(sizeof(CameraStateData) == 0x8);
#else
		static_assert(sizeof(CameraStateData) == 0x8);
#endif
		class State
		{
		public:
			struct RUNTIME_DATA
			{
#if defined(EXCLUSIVE_SKYRIM_VR)  // VR
#	define RUNTIME_DATA_CONTENT                                                                                               \
		uint32_t                   firstCameraStateIndex;                /*	058 VR   only ?*/                                  \
		NiPointer<NiSourceTexture> defaultTextureBlack;                  /* SE 058, AE,VR 060 - "BSShader_DefHeightMap"*/      \
		NiPointer<NiSourceTexture> defaultTextureWhite;                  /* SE 060, AE,VR 068 */                               \
		NiPointer<NiSourceTexture> defaultTextureGrey;                   /* SE 068, AE,VR 070 */                               \
		NiPointer<NiSourceTexture> defaultHeightMap;                     /* SE 070, AE,VR 078 */                               \
		NiPointer<NiSourceTexture> defaultReflectionCubeMap;             /* SE 078, AE,VR 080 */                               \
		NiPointer<NiSourceTexture> defaultFaceDetailMap;                 /* SE 080, AE,VR 088 */                               \
		NiPointer<NiSourceTexture> defaultTexEffectMap;                  /* SE 088, AE,VR 090 */                               \
		NiPointer<NiSourceTexture> defaultTextureNormalMap;              /* SE 090, AE,VR 098 */                               \
		NiPointer<NiSourceTexture> defaultTextureDitherNoiseMap;         /* SE 098, AE,VR 0a0 */                               \
		BSTArray<CameraStateData>  cameraDataCacheA;                     /* SE 0a0, AE,VR 0a8 */                               \
		std::uint32_t              unk0C0;                               /* SE 0a8, AE,VR 0c0 */                               \
		float                      haltonSequence[2][8];                 /* SE 0c0, AE,VR 0c4 (2, 3) Halton Sequence points */ \
		float                      dynamicResolutionWidthRatio;          /* SE 0c4, AE,VR 104 */                               \
		float                      dynamicResolutionHeightRatio;         /* SE 104, AE,VR 108 */                               \
		float                      dynamicResolutionPreviousWidthRatio;  /* SE 108, AE,VR 10c */                               \
		float                      dynamicResolutionPreviousHeightRatio; /* SE 10c, AE,VR 110 */                               \
		std::uint32_t              dynamicResolutionIncreaseFrameWaited; /* SE 110, AE,VR 114 */                               \
		volatile std::int32_t      dynamicResolutionLock;                /* SE 114, AE,VR 118 */                               \
		bool                       canIncreaseDynamicResolution;         /* SE 118, AE,VR 11c */                               \
		bool                       canDecreaseDynamicResolution;         /* SE 119, AE,VR 11d */                               \
		bool                       canChangeDynamicResolution;           /* SE 120, AE,VR 11e */

#else
#	define RUNTIME_DATA_CONTENT                                                                                               \
		NiPointer<NiSourceTexture> defaultTextureBlack;                  /* SE 058, AE,VR 060 - "BSShader_DefHeightMap"*/      \
		NiPointer<NiSourceTexture> defaultTextureWhite;                  /* SE 060, AE,VR 068 */                               \
		NiPointer<NiSourceTexture> defaultTextureGrey;                   /* SE 068, AE,VR 070 */                               \
		NiPointer<NiSourceTexture> defaultHeightMap;                     /* SE 070, AE,VR 078 */                               \
		NiPointer<NiSourceTexture> defaultReflectionCubeMap;             /* SE 078, AE,VR 080 */                               \
		NiPointer<NiSourceTexture> defaultFaceDetailMap;                 /* SE 080, AE,VR 088 */                               \
		NiPointer<NiSourceTexture> defaultTexEffectMap;                  /* SE 088, AE,VR 090 */                               \
		NiPointer<NiSourceTexture> defaultTextureNormalMap;              /* SE 090, AE,VR 098 */                               \
		NiPointer<NiSourceTexture> defaultTextureDitherNoiseMap;         /* SE 098, AE,VR 0a0 */                               \
		BSTArray<CameraStateData>  cameraDataCacheA;                     /* SE 0a0, AE,VR 0a8 */                               \
		std::uint32_t              unk0C0;                               /* SE 0a8, AE,VR 0c0 */                               \
		float                      haltonSequence[2][8];                 /* SE 0c0, AE,VR 0c4 (2, 3) Halton Sequence points */ \
		float                      dynamicResolutionWidthRatio;          /* SE 0c4, AE,VR 104 */                               \
		float                      dynamicResolutionHeightRatio;         /* SE 104, AE,VR 108 */                               \
		float                      dynamicResolutionPreviousWidthRatio;  /* SE 108, AE,VR 10c */                               \
		float                      dynamicResolutionPreviousHeightRatio; /* SE 10c, AE,VR 110 */                               \
		std::uint32_t              dynamicResolutionIncreaseFrameWaited; /* SE 110, AE,VR 114 */                               \
		volatile std::int32_t      dynamicResolutionLock;                /* SE 114, AE,VR 118 */                               \
		bool                       canIncreaseDynamicResolution;         /* SE 118, AE,VR 11c */                               \
		bool                       canDecreaseDynamicResolution;         /* SE 119, AE,VR 11d */                               \
		bool                       canChangeDynamicResolution;           /* SE 120, AE,VR 11e */
#endif
				RUNTIME_DATA_CONTENT;
			};
#if !defined(ENABLE_SKYRIM_VR)  // Non-VR
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xA4);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xAC);
#else
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xA4);
#endif
			[[nodiscard]] static State* GetSingleton()
			{
				REL::Relocation<State*> singleton{ RELOCATION_ID(524998, 411479) };
				return singleton.get();
			}

			CameraStateData* FindCameraDataCache(const NiCamera* a_camera, bool a_useJitter)
			{
				for (uint32_t i = 0; i < GetRuntimeData().cameraDataCacheA.size(); i++) {
					if (GetRuntimeData().cameraDataCacheA[i].referenceCamera == a_camera &&
						GetRuntimeData().cameraDataCacheA[i].GetCameraStateRuntimeData().useJitter == a_useJitter)
						return &GetRuntimeData().cameraDataCacheA[i];
				}
				return nullptr;
			}

			[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
			{
				if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
					if (REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_629) != std::strong_ordering::less) {
						return REL::RelocateMember<RUNTIME_DATA>(this, 0x60);
					}
				}
				return REL::RelocateMember<RUNTIME_DATA>(this, 0x58, 0x60);
			}

			[[nodiscard]] inline const RUNTIME_DATA& GetRuntimeData() const noexcept
			{
				if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
					if (REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_629) != std::strong_ordering::less) {
						return REL::RelocateMember<RUNTIME_DATA>(this, 0x60);
					}
				}
				return REL::RelocateMember<RUNTIME_DATA>(this, 0x58, 0x60);
			}

			[[nodiscard]] inline bool GetDoubleDynamicResolutionAdjustmentFrequency() noexcept
			{
				if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
					return REL::RelocateMember<bool>(this, 0x57);
				}
				return false;
			}
			// members
			NiPointer<NiSourceTexture> defaultTextureProjNoiseMap;         // 000
			NiPointer<NiSourceTexture> defaultTextureProjDiffuseMap;       // 008
			NiPointer<NiSourceTexture> defaultTextureProjNormalMap;        // 010
			NiPointer<NiSourceTexture> defaultTextureProjNormalDetailMap;  // 018
			std::uint32_t              unk20;                              // 020
			std::uint32_t              screenWidth;                        // 024
			std::uint32_t              screenHeight;                       // 028
			std::uint32_t              frameBufferViewport[2];             // 02C
			std::uint32_t              unk34;                              // 034
			std::uint32_t              unk38;                              // 038
			std::uint32_t              unk03C;                             // 03C
			std::uint32_t              unk040;                             // 040
			float                      projectionPosScaleX;                // 044
			float                      projectionPosScaleY;                // 048
			std::uint32_t              frameCount;                         // 04C
			bool                       insideFrame;                        // 050
			bool                       letterbox;                          // 051
			bool                       unk052;                             // 052
			bool                       compiledShaderThisFrame;            // 053
			bool                       useEarlyZ;                          // 054
			bool                       unk055;                             // 055
#ifndef ENABLE_SKYRIM_AE
			RUNTIME_DATA_CONTENT;  // 058, AE,VR 060
#endif
		};
#if defined(EXCLUSIVE_SKYRIM_SE)  // SE
		static_assert(sizeof(State) == 0x118);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, letterbox) == 0x51);
		static_assert(offsetof(State, useEarlyZ) == 0x54);
		static_assert(offsetof(State, defaultTextureBlack) == 0x58);
		static_assert(offsetof(State, defaultTextureWhite) == 0x60);
		static_assert(offsetof(State, cameraDataCacheA) == 0xa0);
		static_assert(offsetof(State, dynamicResolutionWidthRatio) == 0x0fc);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
		static_assert(sizeof(State) == 0x120);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, letterbox) == 0x51);
		static_assert(offsetof(State, useEarlyZ) == 0x54);
		static_assert(offsetof(State, defaultTextureBlack) == 0x60);
		static_assert(offsetof(State, defaultTextureWhite) == 0x68);
		static_assert(offsetof(State, cameraDataCacheA) == 0xa8);
		static_assert(offsetof(State, dynamicResolutionWidthRatio) == 0x104);
#else
		static_assert(sizeof(State) == 0x58);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, letterbox) == 0x51);
		static_assert(offsetof(State, useEarlyZ) == 0x54);
#endif
	}
}
#undef RUNTIME_DATA_CONTENT
