#include <Geode/Geode.hpp>//Include the Geode headers.
#include <Geode/utils/web.hpp>//Include the Geode headers.
using namespace geode::prelude;//cocos2d and all Geode namespaces

#include "SimpleIni.h"
#include "Header1.hpp"

#ifndef GEODE_IS_WINDOWS
#define GetKeyState(asd) (short)asd
#endif

#include <regex>
//funny layers
class MainLevelsEditorLayer : public CCLayer {
public:
    GJGameLevel* m_tar;
    static auto create() {
        auto __this = new MainLevelsEditorLayer;
        if (!__this->init()) CC_SAFE_DELETE(__this);
        //setup
        __this->setKeypadEnabled(true);
        __this->setTouchEnabled(true);
        //BaseContent
        {
            //bg
            CCSprite* backgroundSprite = CCSprite::create("GJ_gradientBG.png");
            backgroundSprite->setScaleX(CCDirector::sharedDirector()->getWinSize().width / backgroundSprite->getContentSize().width);
            backgroundSprite->setScaleY(CCDirector::sharedDirector()->getWinSize().height / backgroundSprite->getContentSize().height);
            backgroundSprite->setAnchorPoint({ 0, 0 });
            backgroundSprite->setColor({ 70, 80, 90 });
            __this->addChild(backgroundSprite, -2);
            //gauntletCorner_001
            CCSprite* gauntletCorner_001 = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
            gauntletCorner_001->setPosition({ 0, 0 });
            gauntletCorner_001->setRotation(0);
            gauntletCorner_001->setAnchorPoint({ 0.f,0.f });
            __this->addChild(gauntletCorner_001);//add gauntletCorner_001
            //gauntletCorner_002
            CCSprite* gauntletCorner_002 = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
            gauntletCorner_002->setPosition({
                CCDirector::sharedDirector()->getWinSize().width,
                0.f
                });
            gauntletCorner_002->setScaleX(-1.f);
            gauntletCorner_002->setAnchorPoint({ 0.f,0.f });
            __this->addChild(gauntletCorner_002);//add gauntletCorner_002
            //gauntletCorner_003
            CCSprite* gauntletCorner_003 = CCSprite::createWithSpriteFrameName("gauntletCorner_001.png");
            gauntletCorner_003->setPosition({
                (CCDirector::sharedDirector()->getWinSize().width),
                (CCDirector::sharedDirector()->getWinSize().height)
                });
            gauntletCorner_003->setRotation(180.f);
            gauntletCorner_003->setAnchorPoint({ 0.f,0.f });
            __this->addChild(gauntletCorner_003);//add gauntletCorner_003
            //backBtn
            auto backBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"), __this, menu_selector(MainLevelsEditorLayer::onBack));
            backBtn->setID("backBtn");
            auto menuBack = CCMenu::createWithItem(backBtn);
            menuBack->setPosition({ 25, CCDirector::sharedDirector()->getWinSize().height - 25 });
            __this->addChild(menuBack);
        };
        return __this;
    }
    void onBack(CCObject* object) {
        CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
    }
    void keyBackClicked() {
        auto backBtn = dynamic_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("backBtn"));
        backBtn->activate();
    }
    void openMe(CCObject*) {
        auto scene = CCScene::create();
        auto me = create();
        scene->addChild(me, 0, 3228);
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    }
    //LevelEditor
    void editorSave(CCObject*) {
        geode::createQuickPopup(
            "Save By Inputs?", 
            "Values u had wrote will be saved in ini file of this level", 
            "No", "Yes", 
            [this](void*, bool asd) {
            if (!asd) return;
            //nodes
            auto levelNameInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("levelNameInput"));
            auto difficultyInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("difficultyInput"));
            auto starsInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("starsInput"));
            auto audioTrackInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("audioTrackInput"));

            auto pGJGameLevel = m_tar;

            std::string MainSection = fmt::format("Level Setup");
            std::string IniPath = FilePathFromModFolder(fmt::format("levels/setup/{}.ini", m_tar->m_levelID.value()));

            CSimpleIni Ini;
            Ini.LoadFile(IniPath.c_str());

            Ini.SetValue(
                MainSection.c_str(),
                "LevelName",
                levelNameInput->getString().c_str(),
                "; Level Name"
            );
            m_tar->m_levelName = difficultyInput->getString();
            Ini.SetValue(
                MainSection.c_str(),
                "difficulty",
                difficultyInput->getString().c_str(),
                "; Difficulties that LevelPage layer supports:\n"
                "; undef = 0,\n"
                "; Easy = 1,\n"
                "; Normal = 2,\n"
                "; Hard = 3,\n"
                "; Harder = 4,\n"
                "; Insane = 5,\n"
                "; Demon = 6"
            );
            m_tar->m_difficulty = (GJDifficulty)stoi(difficultyInput->getString());
            Ini.SetValue(
                MainSection.c_str(),
                "stars",
                starsInput->getString().c_str(),
                "; Stars"
            );
            m_tar->m_stars = stoi(starsInput->getString());
            Ini.SetValue(
                MainSection.c_str(),
                "audioTrack",
                audioTrackInput->getString().c_str(),
                "; Audio Track ID"
            );
            m_tar->m_audioTrack = stoi(audioTrackInput->getString());

            Ini.SaveFile(IniPath.c_str());
            auto ntfyUpdated = Notification::create("Saved...", NotificationIcon::None);
            ntfyUpdated->show();
            
            });
    }
    void editorReset(CCObject* asd) {
        auto pop = geode::createQuickPopup(
            "Reset Inputs?", 
            "Set inputs values by ini files\n"
            "<cr>i mean its not restore original level setup</c>", 
            "No", "Yes", 
            [this](void*, bool asd) {
            if (!asd) return;

            //nodes
            auto levelNameInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("levelNameInput"));
            auto difficultyInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("difficultyInput"));
            auto starsInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("starsInput"));
            auto audioTrackInput = dynamic_cast<InputNode*>(this->getChildByIDRecursive("audioTrackInput"));

            //setup inputs

            std::string MainSection = fmt::format("Level Setup");
            std::string IniPath = FilePathFromModFolder(fmt::format("levels/setup/{}.ini", m_tar->m_levelID.value()));

            CSimpleIni Ini;
            Ini.LoadFile(IniPath.c_str());

            levelNameInput->setString(Ini.GetValue(MainSection.c_str(), "LevelName"));
            difficultyInput->setString(Ini.GetValue(MainSection.c_str(), "difficulty"));
            starsInput->setString(Ini.GetValue(MainSection.c_str(), "stars"));
            audioTrackInput->setString(Ini.GetValue(MainSection.c_str(), "audioTrack"));

            });
        if (!asd) pop->onBtn2(asd);
    }
    void editorEditLevel(CCObject* asd) {
        auto pop = geode::createQuickPopup(
            "Main Level Editor Notice",
            "Yea, u will get Main Level Editor...\n"
            "Sooo saving here <cg>works</c> and <cr>actually writes level data in file</c>.\n"
            "<co>User Coins</c> will be replaced by <cy>Gold Coins</c>\n"
            "<cr>Audio Track doesn't saving!</c>",
            "Let me go", nullptr,
            460.f,
            [this](void*, bool a) {
                auto levelForEditor = PlayLayer::create(m_tar, 0, 0)->m_level;
                //levelString
                {
                    std::string level_data = levelForEditor->m_levelString;
                    std::string decompressed = ZipUtils::decompressString(level_data, false, 0);
                    //user coin id 1329, 142 golden coin
                    decompressed = std::regex_replace(decompressed, std::regex(",142,"), ",1329,");
                    level_data = ZipUtils::compressString(decompressed, false, 0);
                    levelForEditor->m_levelString = level_data;
                };
                auto scene = CCScene::create();
                auto editor = LevelEditorLayer::create(levelForEditor, 0);
                editor->addChild(CCNode::create(), 999, 1971);
                editor->getChildByTag(1971)->setID("MainLevelEditorMark");
                scene->addChild(editor);
                CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
            }
        );
    }
    void editorOnBack(CCObject* object) {
        auto emptyScene = CCScene::create();
        auto scene = LevelSelectLayer::scene(this->getTag() - 1);
        if (this->getTag() >= 5000 and this->getTag() < 5100) {
            emptyScene->addChild(LevelAreaInnerLayer::create(1));
            scene = emptyScene;
        };
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
    }
    static void openLevelEditor(GJGameLevel* tar) {
        auto scene = CCScene::create();
        auto me = create();
        me->m_tar = tar;
        dynamic_cast<CCMenuItemSpriteExtra*>(me->getChildByIDRecursive("backBtn"))
            ->setTarget(me, menu_selector(MainLevelsEditorLayer::editorOnBack));
        {
            //create menu
            auto inputsContainer = CCMenu::create();
            //title
            {
                auto title = CCLabelBMFont::create("Edit Level", "goldFont.fnt");
                title->setID("title");
                inputsContainer->addChild(title);
            };
            //levelNameInput
            {
                auto levelNameInputLabel = CCLabelTTF::create("Level Name:", "Comic Sans MS", 13.f);
                levelNameInputLabel->setID("levelNameInputLabel");
                inputsContainer->addChild(levelNameInputLabel);
                auto levelNameInput = InputNode::create(310.f, "Level Name", "chatFont.fnt");
                levelNameInput->setID("levelNameInput");
                inputsContainer->addChild(levelNameInput);
            };
            //difficultyInput
            {
                auto difficultyInputLabel = CCLabelTTF::create("Difficulty:", "Comic Sans MS", 13.f);
                difficultyInputLabel->setID("difficultyInputLabel");
                inputsContainer->addChild(difficultyInputLabel);
                auto difficultyInput = InputNode::create(310.f, "Difficulty", "chatFont.fnt");
                difficultyInput->setID("difficultyInput");
                difficultyInput->getInput()->setAllowedChars("1234567890");
                inputsContainer->addChild(difficultyInput);
            };
            //starsInput
            {
                auto starsInputLabel = CCLabelTTF::create("Stars:", "Comic Sans MS", 13.f);
                starsInputLabel->setID("starsInputLabel");
                inputsContainer->addChild(starsInputLabel);
                auto starsInput = InputNode::create(310.f, "Stars", "chatFont.fnt");
                starsInput->setID("starsInput");
                starsInput->getInput()->setAllowedChars("1234567890");
                inputsContainer->addChild(starsInput);
            };
            //audioTrackInput
            {
                auto audioTrackInputLabel = CCLabelTTF::create("Audio Track:", "Comic Sans MS", 13.f);
                audioTrackInputLabel->setID("audioTrackInputLabel");
                inputsContainer->addChild(audioTrackInputLabel);
                auto audioTrackInput = InputNode::create(310.f, "Audio Track", "chatFont.fnt");
                audioTrackInput->setID("audioTrackInput");
                audioTrackInput->getInput()->setAllowedChars("1234567890");
                inputsContainer->addChild(audioTrackInput);
            };
            //con troll btns
            {
                auto asd = ButtonSprite::create("Reset");
                auto kfc = CCMenuItemSpriteExtra::create(asd, me, menu_selector(MainLevelsEditorLayer::editorReset));
                auto asd2 = ButtonSprite::create("Save");
                auto kfc2 = CCMenuItemSpriteExtra::create(asd2, me, menu_selector(MainLevelsEditorLayer::editorSave));
                auto container = CCMenu::create(kfc, kfc2, nullptr);
                container->setContentHeight(32.f);
                container->alignItemsHorizontallyWithPadding(12.f);
                CCMenuItemSpriteExtra* editLevel;
                {
                    auto hi = geode::AccountButtonSprite::create(
                        CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png"),
                        AccountBaseColor::Gray
                    );
                    hi->setScale(0.8f);
                    typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setScale(1.1f);
                    editLevel = CCMenuItemSpriteExtra::create(hi, me, menu_selector(MainLevelsEditorLayer::editorEditLevel));
                    editLevel->setPositionX(208.f);
                    editLevel->setPositionY(-10.f);
                    container->addChild(editLevel);
                }
                inputsContainer->addChild(container);
            }
            //update and add menu
            inputsContainer->alignItemsVertically();
            inputsContainer->setAnchorPoint(CCPointZero);
            inputsContainer->setScale(0.9f);
            me->addChild(inputsContainer);
            //bg
            {
                auto bg = CCScale9Sprite::create("GJ_square05.png");
                bg->setID("bg");
                bg->setContentSize({366.f, 318.f});
                inputsContainer->addChild(bg, -1, 57290);
            };
            //yea setup inputs
            me->editorReset(nullptr);//nullptr is means click on btn2 instatnly
        }
        scene->addChild(me, 0, tar->m_levelID);
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    }
};
#include <Geode/modify/EditorPauseLayer.hpp>
class $modify(EditorPauseLayerExt, EditorPauseLayer) {
    bool isMainLevelEditor() {
        return this->m_editorLayer->getChildByIDRecursive("MainLevelEditorMark");
    }
    void onSave(cocos2d::CCObject* sender) {
        if (isMainLevelEditor()) {
            this->saveLevel();
            Notification::create("Saving level")->show();
            saveLevelData(this->m_editorLayer->m_level);
            return;
        }
        EditorPauseLayer::onSave(sender);
    };
    void onSaveAndExit(cocos2d::CCObject* sender) {
        if (isMainLevelEditor()) {
            MainLevelsEditorLayer::openLevelEditor(this->m_editorLayer->m_level);
            this->saveLevel();
            Notification::create("Saving level")->show();
            saveLevelData(this->m_editorLayer->m_level);
            return;
        }
        EditorPauseLayer::onSaveAndExit(sender);
    };
    void onSaveAndPlay(cocos2d::CCObject* sender) {
        if (isMainLevelEditor()) if(auto node = dynamic_cast<CCMenuItemSpriteExtra*>(sender)) {
            node->setNormalImage(CCLabelTTF::create("No", "Courier New", 16.f));
            return;
        }
        EditorPauseLayer::onSaveAndPlay(sender);
    };
    void FLAlert_Clicked(FLAlertLayer* p0, bool p1) {
        if (isMainLevelEditor() and p1) return MainLevelsEditorLayer::openLevelEditor(this->m_editorLayer->m_level);
        EditorPauseLayer::FLAlert_Clicked(p0, p1);
    };
};

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(LevelInfoLayerExt, LevelInfoLayer) {
    void SomeSch(float) {
        if (!this) return;
    }
    bool init(GJGameLevel* p0, bool p1) {
        if (Mod::get()->getSettingValue<bool>("COLH"))
            this->schedule(schedule_selector(LevelInfoLayerExt::SomeSch), 0.01f);
        if (Mod::get()->getSettingValue<bool>("SL"))
            p0 = processOutLevelByConfig(p0->m_levelID.value(), p0);
        LevelInfoLayer::init(p0, p1);
        if (Mod::get()->getSettingValue<bool>("UI")) {
            //GJ_copyStateBtn_001
            auto GJ_copyStateBtn_001 = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_copyStateBtn_001.png"),
                this,
                menu_selector(MainLevelsEditorLayer::openMe)
            );
            //pCCMenu
            {
                auto pCCMenu = CCMenu::createWithItem(GJ_copyStateBtn_001);
                pCCMenu->setAnchorPoint(CCSizeZero);
                pCCMenu->setScale(.8f);
                pCCMenu->setPositionY(18.f);
                pCCMenu->setPositionX(CCDirector::get()->getScreenRight() - 110);
                pCCMenu->setID("main levels editor ui");
                this->addChild(pCCMenu, 999, 6282);
            };
        }
        return 1;
	}
    TodoReturn setupLevelInfo() {
        if (Mod::get()->getSettingValue<bool>("SL"))
            this->m_level = processOutLevelByConfig(this->m_level->m_levelID.value(), this->m_level);
        LevelInfoLayer::setupLevelInfo();
    };
};

#include <Geode/modify/EditLevelLayer.hpp>
class $modify(EditLevelLayerExt, EditLevelLayer) {
    bool init(GJGameLevel * p0) {
        auto rtn = EditLevelLayer::init(p0);
        if (Mod::get()->getSettingValue<bool>("UI")) {
            //GJ_copyStateBtn_001
            auto GJ_copyStateBtn_001 = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_copyStateBtn_001.png"),
                this,
                menu_selector(MainLevelsEditorLayer::openMe)
            );
            //pCCMenu
            {
                auto pCCMenu = CCMenu::createWithItem(GJ_copyStateBtn_001);
                pCCMenu->setAnchorPoint(CCSizeZero);
                pCCMenu->setScale(.8f);
                pCCMenu->setPositionY(18.f);
                pCCMenu->setPositionX(CCDirector::get()->getScreenRight() - 85);
                pCCMenu->setID("main levels editor ui");
                this->addChild(pCCMenu, 999, 6282);
            };
        }
        return rtn;
    };
};

#include <Geode/modify/LevelPage.hpp>
class $modify(LevelPageExt, LevelPage) {
    void updateUI(float) {
        if (auto pCCMenu = this->getChildByIDRecursive("main levels editor ui")) {
            pCCMenu->setVisible(this->m_level->m_levelID >= 0);
        }
    }
    void editLevel(CCObject*) {
        MainLevelsEditorLayer::openLevelEditor(this->m_level);
    }
    void deleteLevel(CCObject*) {
        auto g_currentLevel = this->m_level;
        auto g_currentPage = this->m_level->m_levelID.value();
        std::string const& content = {
            fmt::format(
                "<cg>Name:</c> {}" "\n"
                "<cj>ID:</c> {}" "\n"
                "<cy>Stars:</c> {}" "\n"
                "<co>Difficulty:</c> {}" "\n"
                "<cb>Audio Track:</c> {} ({})" "\n"
                "<cr>Delete</c> this level from <co>mod config</c>?",
                g_currentLevel->m_levelName,
                g_currentPage,
                g_currentLevel->m_stars.value(),
                (int)g_currentLevel->m_difficulty,
                LevelTools::getAudioTitle(g_currentLevel->m_audioTrack), g_currentLevel->m_audioTrack
            )
        };
        geode::createQuickPopup("Delete Level", content, "No", "Yes", [this, g_currentPage, g_currentLevel](void*, bool asd) {
            if (!asd) return;
            //level setup
            std::remove(FilePathFromModFolder(fmt::format("levels/setup/{}.ini", g_currentPage)).c_str());
            //data store
            std::remove(FilePathFromModFolder(fmt::format("levels/{}.txt", g_currentPage)).c_str());
            //_PagesSetupPatch
            {
                std::string MainSection = fmt::format("{}", "UpdatePagesSetup");
                std::string IniPath = FilePathFromModFolder("_PagesSetupPatch.ini");
                CSimpleIni Ini;
                Ini.LoadFile(IniPath.c_str());
                Ini.SetLongValue(
                    MainSection.c_str(),
                    "cmp_amount",
                    (Ini.GetLongValue(MainSection.c_str(), "cmp_amount")) - 1,
                    "; level pages adding amount (u cant set more than 127!)"
                );
                Ini.SaveFile(IniPath.c_str());
            };
            //_AudioTracks
            {
                std::string MainSection = fmt::format("{}", g_currentLevel->m_audioTrack);
                std::string IniPath = FilePathFromModFolder("_AudioTracks.ini");

                CSimpleIniA Ini;
                Ini.LoadFile(IniPath.c_str());

                Ini.Delete(MainSection.c_str(), NULL);

                Ini.SaveFile(IniPath.c_str());
            };
            //asshell
            {
                //level setup
                for (size_t levelID = 0; levelID < 127; levelID++) {
                    if (levelID > g_currentPage and levelID < 127) {
                        std::rename(
                            FilePathFromModFolder(fmt::format("levels/setup/{}.ini", levelID)).c_str(),
                            FilePathFromModFolder(fmt::format("levels/setup/{}.ini", levelID - 1)).c_str()
                        );
                    };
                }
                //level data
                for (size_t levelID = 0; levelID < 127; levelID++) {
                    if (levelID > g_currentPage) {
                        if (
                            !std::filesystem::exists(FilePathFromModFolder(fmt::format("levels/{}.txt", levelID)))
                            and std::filesystem::exists(fmt::format("Resources/levels/{}.txt", levelID))
                            )
                            std::filesystem::copy_file(
                                fmt::format("Resources/levels/{}.txt", levelID).c_str(),
                                FilePathFromModFolder(fmt::format("levels/{}.txt", levelID - 1)).c_str()
                            );
                        else
                            std::rename(
                                FilePathFromModFolder(fmt::format("levels/{}.txt", levelID)).c_str(),
                                FilePathFromModFolder(fmt::format("levels/{}.txt", levelID - 1)).c_str()
                            );
                    }
                }
            }
            //a
            auto ntfy = Notification::create(fmt::format("{} deleted...", g_currentLevel->m_levelName.data()).data(), NotificationIcon::Warning);
            ntfy->show();
            CCDirector::get()->pushScene(LevelSelectLayer::scene(g_currentPage - 1));
            });
    }
    static LevelPage* create(GJGameLevel * p0) {
        auto rtn = LevelPage::create(p0);
        if (Mod::get()->getSettingValue<bool>("UI")) {
            rtn->schedule(schedule_selector(LevelPageExt::updateUI));
            CCMenuItemSpriteExtra* deleteLevel;
            {
                //Node
                auto hi = geode::AccountButtonSprite::create(
                    CCSprite::createWithSpriteFrameName("edit_delBtn_001.png"),
                    AccountBaseColor::Gray
                );
                hi->setScale(0.8f);
                typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setScale(1.1f);
                deleteLevel = CCMenuItemSpriteExtra::create(hi, rtn, menu_selector(LevelPageExt::deleteLevel));
            };
            CCMenuItemSpriteExtra* editLevel;
            {
                //Node
                auto hi = geode::AccountButtonSprite::create(
                    CCSprite::createWithSpriteFrameName("d_cogwheel_04_001.png"),
                    AccountBaseColor::Gray
                );
                hi->setScale(0.8f);
                typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setScale(1.1f);
                typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setColor({ 20, 20, 20 });
                hi->addChild(typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0)));
                editLevel = CCMenuItemSpriteExtra::create(hi, rtn, menu_selector(LevelPageExt::editLevel));
            };
            auto pCCMenu = CCMenu::create(deleteLevel, editLevel, nullptr);
            {
                pCCMenu->setAnchorPoint(CCSizeZero);
                pCCMenu->setScale(.8f);
                pCCMenu->setPositionY(CCDirector::get()->getScreenTop() / 2);
                pCCMenu->setPositionX(CCDirector::get()->getScreenRight() - 85);
                pCCMenu->setID("main levels editor ui");
                pCCMenu->alignItemsVerticallyWithPadding(12.f);
                rtn->addChild(pCCMenu, 999, 6282);
            };
        }
        return rtn;
    };
};

#include <Geode/modify/LevelAreaInnerLayer.hpp>
class $modify(LevelAreaInnerLayerExt, LevelAreaInnerLayer) {
    class EditBtn : public CCMenuItemSpriteExtra {
    public:
        void editLevel(CCObject* asneeeeder) {
            MainLevelsEditorLayer::openLevelEditor(
                PlayLayer::create(
                    LevelTools::getLevel(asneeeeder->getTag(), false),
                    0, 0
                )->m_level
            );
        }
        static auto createAndSetup(CCNode* door) {
            //Node
            auto hi = geode::AccountButtonSprite::create(
                CCSprite::createWithSpriteFrameName("d_cogwheel_04_001.png"),
                AccountBaseColor::Gray
            );
            hi->setScale(0.4f);
            typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setScale(1.1f);
            typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0))->setColor({ 20, 20, 20 });
            hi->addChild(typeinfo_cast<CCSprite*>(hi->getChildren()->objectAtIndex(0)));
            //create
            auto me = (EditBtn*)create(hi, door, menu_selector(EditBtn::editLevel));
            me->setTag(door->getTag());
            door->getParent()->addChild(me, 999);
            me->setPositionX(door->getPosition().x + 40);
            me->setPositionY(door->getPosition().y + 45);
            return me;
        }
    };
    static LevelAreaInnerLayer* create(bool p0) {
        auto rtn = LevelAreaInnerLayer::create(p0);
        if (Mod::get()->getSettingValue<bool>("UI")) {
            auto mainnode = dynamic_cast<CCNode*>(rtn->getChildren()->objectAtIndex(1));
            if (mainnode) {
                for (int index = 0; index < mainnode->getChildrenCount(); ++index) {
                    auto obj = mainnode->getChildren()->objectAtIndex(index);
                    if (obj) {
                        auto menu = dynamic_cast<CCMenu*>(obj);
                        if (menu) {
                            if (menu->getChildByTag(5001)) {
                                EditBtn::createAndSetup(menu->getChildByTag(5001));
                                EditBtn::createAndSetup(menu->getChildByTag(5002));
                                EditBtn::createAndSetup(menu->getChildByTag(5003));
                                EditBtn::createAndSetup(menu->getChildByTag(5004));
                            };
                        };
                    };
                };
            }
        }
        return rtn;
    }
};