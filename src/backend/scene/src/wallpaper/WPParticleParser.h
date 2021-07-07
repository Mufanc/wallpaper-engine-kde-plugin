#pragma once
#include "Particle/ParticleEmitter.h"
#include "wpscene/WPParticleObject.h"

namespace wallpaper {
class WPParticleParser {
public:
	typedef std::function<float()> RandomFn;
	static ParticleInitOp genParticleInitOp(const wpscene::Initializer&, RandomFn);
	static ParticleOperatorOp genParticleOperatorOp(const nlohmann::json&, RandomFn);
};
}