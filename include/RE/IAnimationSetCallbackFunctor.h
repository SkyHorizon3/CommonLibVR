#pragma once


namespace RE
{
	class IAnimationSetCallbackFunctor
	{
	public:
		virtual ~IAnimationSetCallbackFunctor();	// 00

		// add
		virtual void	Unk_01(void);				// 01
	};
	STATIC_ASSERT(sizeof(IAnimationSetCallbackFunctor) == 0x8);
}
