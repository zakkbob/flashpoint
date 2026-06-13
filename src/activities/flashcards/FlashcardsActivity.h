#pragma once
#include "activities/Activity.h"

class FlashcardsActivity final : public Activity {
public:
	explicit FlashcardsActivity(GfxRenderer& renderer, MappedInputManager& mappedInput) : Activity("Flashcards", renderer, mappedInput)  {};

	void onEnter() override;
};
