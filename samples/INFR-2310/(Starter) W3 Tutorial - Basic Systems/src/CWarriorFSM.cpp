#include "CWarriorFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	//String labels for animations.
	const std::string CWarriorFSM::idleClip = "idle1";
	const std::string CWarriorFSM::runClip = "walk1";
	const std::string CWarriorFSM::attackClip = "attack1";

	CWarriorFSM::CWarriorFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CWarriorFSM::SetState(CWarriorFSM::AnimState state)
	{
		m_state = state;

		//Gets the animator from the entity.
		auto& animator = m_owner->Get<CSpriteAnimator>();

		ClearTriggers();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:
			animator.PlayLoop(idleClip);
			break;
		case AnimState::RUN:
			animator.PlayLoop(runClip);
			break;
		default:
			animator.PlayOnce(attackClip);
			break;
		}
	}


	void CWarriorFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:

			if (GetVariable("moving1"))
				SetState(AnimState::RUN);
			if (GetTrigger("attack1"))
				SetState(AnimState::ATTACK);
			break;

		case AnimState::RUN:

			if (!GetVariable("moving1"))
				SetState(AnimState::IDLE);

			break;
		case AnimState::ATTACK:
			if (!GetVariable("moving1") && m_owner->Get<CSpriteAnimator>().IsDone())
			{
				SetState(AnimState::IDLE);
			}
			break;
		}
	}
}