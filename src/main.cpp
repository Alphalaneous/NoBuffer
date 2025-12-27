#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(MyGJBaseGameLayer, GJBaseGameLayer) {

    struct Fields {
        bool m_allowHoldJumps = false;
        bool m_wasOnGroundP1 = false;
        bool m_wasOnGroundP2 = false;
    };

    bool init() {
        if (!GJBaseGameLayer::init()) return false;
        auto fields = m_fields.self();
        fields->m_allowHoldJumps = Mod::get()->getSettingValue<bool>("allow-hold-jumps");
        return true;
    }

    void update(float dt) {
        GJBaseGameLayer::update(dt);
        auto fields = m_fields.self();

        if (!m_player1->m_isShip && !m_player1->m_isDart && !m_player1->m_isRobot && !m_player1->m_isDashing && !fields->m_wasOnGroundP1) {
            GJBaseGameLayer::handleButton(false, 1, true);
        }

        if (m_level->m_twoPlayerMode) {
            if (!m_player2->m_isShip && !m_player2->m_isDart && !m_player2->m_isRobot && !m_player2->m_isDashing && !fields->m_wasOnGroundP2) {
                GJBaseGameLayer::handleButton(false, 1, false);
            }
        }
    }

    bool isCube(PlayerObject* player) {
        return !player->m_isShip && !player->m_isDart && !player->m_isBird && !player->m_isBall && !player->m_isSwing && !player->m_isSpider && !player->m_isRobot;
    }

    void handleButton(bool down, int button, bool isPlayer1) {
        if (down) {
            auto fields = m_fields.self();
            if (fields->m_allowHoldJumps) {
                if (isPlayer1) {
                    fields->m_wasOnGroundP1 = m_player1->m_isOnGround && isCube(m_player1);
                }
                else {
                    fields->m_wasOnGroundP2 = m_player2->m_isOnGround && isCube(m_player2);
                }
            }
        }
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    }
};