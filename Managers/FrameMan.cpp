#include "FrameMan.h"

#include "ContentFile.h"
#include "Timer.h"
#include "Box.h"
#include "PostProcessMan.h"
#include "PrimitiveMan.h"
#include "PerformanceMan.h"
#include "SceneMan.h"
#include "ActivityMan.h"
#include "ConsoleMan.h"
#include "SettingsMan.h"
#include "UInputMan.h"

#include "Entities/SLTerrain.h"
#include "Entities/Scene.h"

#include "GUI/GUI.h"
#include "GUI/SDLGUITexture.h"
#include "GUI/SDLScreen.h"

#include "System/Constants.h"
#include "System/RTEError.h"

#include "System/SDLHelper.h"
#include <SDL2/SDL2_gfxPrimitives.h>

extern bool g_InActivity;

namespace RTE {

	FrameMan::FrameMan() { Clear(); }

	FrameMan::~FrameMan() = default;

	void FrameMan::Clear() {
		m_Window = nullptr;
		m_Renderer = nullptr;

		m_NumScreens = SDL_GetNumVideoDisplays();
		SDL_GetDisplayBounds(0, m_ScreenRes.get());

		m_ResX = 960;
		m_ResY = 540;
		m_ResMultiplier = 1;
		m_NewResX = m_ResX;
		m_NewResY = m_ResY;

		m_ResChanged = false;
		m_Fullscreen = false;
		m_UpscaledFullscreen = false;

		m_HSplit = false;
		m_VSplit = false;
		m_HSplitOverride = false;
		m_VSplitOverride = false;

		for (; !m_TargetStack.empty(); m_TargetStack.pop())
			;
		m_TargetStack.push(nullptr);
	}

	bool FrameMan::IsValidResolution(int width, int height) const {
		return (width >= 640 && height >= 480);
	}

	int FrameMan::CreateWindowAndRenderer() {

		m_Window = SDL_CreateWindow(c_WindowTitle, SDL_WINDOWPOS_CENTERED,
		                            SDL_WINDOWPOS_CENTERED, m_ResX, m_ResY,
		                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |
		                                SDL_WINDOW_INPUT_FOCUS);

		RTEAssert(m_Window != NULL, "Could not create Window because: " + std::string(SDL_GetError()));

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

		RTEAssert(m_Renderer != NULL, "Could not create Renderer because: " + std::string(SDL_GetError()));

		return m_Window && m_Renderer;
	}

	int FrameMan::Initialize() {

		SDL_SetWindowSize(m_Window, m_ResX, m_ResY);

		SetFullscreen(m_Fullscreen);

		// Set integer scaling so we don't get artifacts by rendering subpixels.
		SDL_RenderSetIntegerScale(m_Renderer, SDL_TRUE);

		SDL_RenderSetLogicalSize(m_Renderer, m_ResX, m_ResY);

		// Upscale the resolution to the set multiplier.
		SDL_RenderSetScale(m_Renderer, m_ResMultiplier, m_ResMultiplier);

		SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_Renderer);

		m_Palette = m_PaletteFile.GetAsTexture();

		return 0;
	}

	void FrameMan::Destroy() {
		if (m_Renderer)
			SDL_DestroyRenderer(m_Renderer);
		if (m_Window)
			SDL_DestroyWindow(m_Window);

		Clear();
	}

	void FrameMan::Update() {
		g_PerformanceMan.Update();

		g_PrimitiveMan.ClearPrimitivesQueue();
	}

	void FrameMan::ResetSplitScreens(bool hSplit, bool vSplit) {
		if (!vSplit && !hSplit)
			m_PlayerScreen.reset();
		else {
			int resX = vSplit ? m_ResX / 2 : m_ResX;
			int resY = hSplit ? m_ResY / 2 : m_ResY;

			m_PlayerScreen = std::make_unique<Texture>(m_Renderer, resX, resY);
		}
	}

	int FrameMan::GetPlayerFrameBufferWidth(short) const {
		if (m_PlayerScreen)
			return m_PlayerScreen->getW();

		return m_ResX;
	}

	int FrameMan::GetPlayerFrameBufferHeight(short) const {
		if (m_PlayerScreen)
			return m_PlayerScreen->getH();

		return m_ResY;
	}

	void FrameMan::PushRenderTarget(SDL_Texture *target) {
		if (target) {
			int access;
			SDL_QueryTexture(target, nullptr, &access, nullptr, nullptr);
			RTEAssert(access == SDL_TEXTUREACCESS_TARGET,
			          "Trying to set a render target to non target texture");
		}
		m_TargetStack.push(target);
		SDL_SetRenderTarget(m_Renderer, target);
	}

	void FrameMan::PushRenderTarget(std::shared_ptr<Texture> target) {
		if (!target) {
			PushRenderTarget(nullptr);
			return;
		}
		RTEAssert(target->getAccess() == SDL_TEXTUREACCESS_TARGET, "Trying to set a render target to non target texture");

		m_TargetStack.push(target->getAsRenderTarget());
		SDL_SetRenderTarget(m_Renderer, m_TargetStack.top());
	}

	void FrameMan::PopRenderTarget() {
		RTEAssert(m_TargetStack.size() > 1, "Attempted removing the main renderer");
		m_TargetStack.pop();
		SDL_SetRenderTarget(m_Renderer, m_TargetStack.top());
	}

	void FrameMan::RenderClear() {
		RTEAssert(m_TargetStack.top() == nullptr, "Targets have not been reset!");
		SDL_RenderClear(m_Renderer);
	}

	void FrameMan::RenderPresent() {
		RTEAssert(m_TargetStack.top() == nullptr, "A render target has not been reset!");
		SDL_RenderPresent(m_Renderer);
	}

	void FrameMan::UpdateScreenOffsetForSplitScreen(int playerScreen, Vector &screenOffset) const {
		switch (playerScreen) {
			case Players::PlayerTwo:
				// If both splits, or just VSplit, then in upper right quadrant
				if ((m_VSplit && !m_HSplit) || (m_VSplit && m_HSplit)) {
					screenOffset.SetXY(GetResX() / 2, 0);
				} else {
					// If only HSplit, then lower left quadrant
					screenOffset.SetXY(0, GetResY() / 2);
				}
				break;
			case Players::PlayerThree:
				// Always lower left quadrant
				screenOffset.SetXY(0, GetResY() / 2);
				break;
			case Players::PlayerFour:
				// Always lower right quadrant
				screenOffset.SetXY(GetResX() / 2, GetResY() / 2);
				break;
			default:
				// Always upper left corner
				screenOffset.SetXY(0, 0);
				break;
		}
	}

	void FrameMan::DrawScreenFlash(int playerScreen, SDL_Renderer *renderer) {
	}

	void FrameMan::Draw() {
		// Count how many split screens we'll need
		int screenCount = (m_HSplit ? 2 : 1) * (m_VSplit ? 2 : 1);
		// RTEAssert(screenCount <= 1 || m_PlayerScreen,
		//           "Splitscreen surface not ready when needed!");

		g_PostProcessMan.ClearScreenPostEffects();

		// These accumulate the effects for each player's screen area, and are
		// then transferred to the post-processing lists with the player screen
		// offset applied
		std::list<PostEffect> screenRelativeEffects;
		std::list<Box> screenRelativeGlowBoxes;

		const Activity *pActivity = g_ActivityMan.GetActivity();

		for (int playerScreen = 0; playerScreen < screenCount; ++playerScreen) {
			if (screenCount > 1)
				PushRenderTarget(m_PlayerScreen->getAsRenderTarget());

			SDL_Rect renderSize;
			SDL_RenderGetLogicalSize(m_Renderer, &renderSize.w, &renderSize.h);

			SDLGUITexture playerGUIBitmap;
			screenRelativeEffects.clear();
			screenRelativeGlowBoxes.clear();
			// TODO Multiplayer
			// Update the scene view to line up with a specific screen and then
			// draw it onto the intermediate screen
			g_SceneMan.Update(playerScreen);

			Vector targetPos = g_SceneMan.GetOffset(playerScreen);

			// Adjust the drawing position on the target screen for if the
			// target screen is larger than the scene in non-wrapping dimension.
			// Scene needs to be displayed centered on the target bitmap then,
			// and that has to be adjusted for when drawing to the screen
			if (!g_SceneMan.SceneWrapsX() && renderSize.w > g_SceneMan.GetSceneWidth()) {
				targetPos.m_X += (renderSize.w - g_SceneMan.GetSceneWidth()) / 2;
			}
			if (!g_SceneMan.SceneWrapsY() && renderSize.h > g_SceneMan.GetSceneHeight()) {
				targetPos.m_Y += (renderSize.h - g_SceneMan.GetSceneHeight()) / 2;
			}

#ifdef NETWORK_ENABLED
			//Try to move at the frame buffer copy time to maybe prevent
			// wonkyness
			m_TargetPos[m_NetworkFrameCurrent][playerScreen] = targetPos;
#endif

			// Draw the scene
			if (!IsInMultiplayerMode()) {
				g_SceneMan.Draw(m_Renderer, nullptr, targetPos);
			} else {
#ifdef NETWORK_ENABLED
				clear_to_color(drawScreen, g_MaskColor);
				clear_to_color(drawScreenGUI, g_MaskColor);
				g_SceneMan.Draw(drawScreen, drawScreenGUI, targetPos, true, true);
#endif
			}

			// Get only the scene-relative post effects that affect this
			// player's screen
			if (pActivity) {
				g_PostProcessMan.GetPostScreenEffectsWrapped(targetPos, renderSize.w, renderSize.h,
				                                             screenRelativeEffects, pActivity->GetTeamOfPlayer(pActivity->PlayerOfScreen(playerScreen)));

				g_PostProcessMan.GetGlowAreasWrapped(targetPos, renderSize.w, renderSize.h, screenRelativeGlowBoxes);

#ifdef NETWORK_ENBALED
				if (IsInMultiplayerMode()) {
					g_PostProcessMan.SetNetworkPostEffectsList(
					    playerScreen, screenRelativeEffects);
				}
#endif
			}

			DrawScreenText(playerScreen, playerGUIBitmap);

			// The position of the current draw screen on the backbuffer
			Vector screenOffset;

			// If we are dealing with split screens, then deal with the fact
			// that we need to draw the player screens to different locations on
			// the final buffer
			if (screenCount > 1) {
				UpdateScreenOffsetForSplitScreen(playerScreen, screenOffset);
			}

			DrawScreenFlash(playerScreen, m_Renderer);

			PopRenderTarget();
			if (!IsInMultiplayerMode()) {
				if (m_PlayerScreen)
					m_PlayerScreen->render(m_Renderer, screenOffset.GetFloorIntX(), screenOffset.GetFloorIntY());

				g_PostProcessMan.AdjustEffectsPosToPlayerScreen(playerScreen, renderSize.w, renderSize.h, screenOffset, screenRelativeEffects, screenRelativeGlowBoxes);
			}
		}

		// Clears the pixels that have been revealed from the unseen layers
		g_SceneMan.ClearSeenPixels();

		SDL_Rect renderSize;
		SDL_RenderGetLogicalSize(m_Renderer, &renderSize.w, &renderSize.h);

		if (!IsInMultiplayerMode()) {
			// Draw separating lines for split-screens
			if (m_HSplit) {
				hlineColor(m_Renderer, 0, (renderSize.h / 2) - 1, renderSize.w - 1, g_BlackColor);
				hlineColor(m_Renderer, 0, (renderSize.h / 2), renderSize.w - 1, g_BlackColor);
			}
			if (m_VSplit) {
				vlineColor(m_Renderer, (renderSize.w / 2) - 1, 0, renderSize.h - 1, g_BlackColor);
				vlineColor(m_Renderer, (renderSize.w / 2), 0, renderSize.h - 1, g_BlackColor);
			}
#ifdef NETWORK_ENABLED
			// Replace 8 bit backbuffer contents with network received image
			// before post-processing as it is where this buffer is copied to 32
			// bit buffer
			if (GetDrawNetworkBackBuffer()) {
				m_NetworkBitmapLock[0].lock();

				blit(m_NetworkBackBufferFinal8[m_NetworkFrameReady][0],
				     m_BackBuffer8, 0, 0, 0, 0, m_BackBuffer8->w,
				     m_BackBuffer8->h);
				masked_blit(
				    m_NetworkBackBufferFinalGUI8[m_NetworkFrameReady][0],
				    m_BackBuffer8, 0, 0, 0, 0, m_BackBuffer8->w,
				    m_BackBuffer8->h);

				if (g_UInputMan.FlagAltState() || g_UInputMan.FlagCtrlState() ||
				    g_UInputMan.FlagShiftState()) {
					g_PerformanceMan.DrawCurrentPing();
				}

				m_NetworkBitmapLock[0].unlock();
			}
#endif
		}

#ifdef NETWORK_ENABLED
		if (IsInMultiplayerMode()) {
			PrepareFrameForNetwork();
		}
#endif

		if (g_InActivity) {
			g_PostProcessMan.PostProcess();
		}

		// Draw the console on top of everything
		g_ConsoleMan.Draw(m_Renderer);

#ifdef DEBUG_BUILD
		// Draw scene seam
		vlineColor(m_Renderer, 0, 0, g_SceneMan.GetSceneHeight(), 0x45F1FFFF);
#endif

		// Reset the frame timer so we can measure how much it takes until next
		// frame being drawn
		g_PerformanceMan.ResetFrameTimer();
	}

	uint32_t FrameMan::GetPixelFormat() const {
		return SDL_GetWindowPixelFormat(m_Window);
	}

	int FrameMan::SwitchResolutionMultiplier(unsigned char multiplier) {
		if (multiplier <= 0 || multiplier > 4 || multiplier == m_ResMultiplier)
			return -1;

		if (m_ResX > m_ScreenRes->w / multiplier || m_ResY > m_ScreenRes->h / multiplier) {
			ShowMessageBox("Requested resolution multiplier will result in game window exceeding display bounds!\nNo change will be made!");
			return -1;
		}

		if (SDL_RenderSetScale(m_Renderer, multiplier, multiplier) != 0) {
			g_ConsoleMan.PrintString("ERROR: " + std::string(SDL_GetError()));
			return 1;
		}

		SDL_RenderGetLogicalSize(m_Renderer, &m_ResX, &m_ResY);

		g_SettingsMan.UpdateSettingsFile();

		g_FrameMan.RenderPresent();

		return 0;
	}

	void FrameMan::SwitchToFullscreen(bool upscaled, bool endActivity) {
		SetFullscreen(true);
		m_ResMultiplier = upscaled ? 2 : 1;
		SwitchResolution(m_ScreenRes->w, m_ScreenRes->h, m_ResMultiplier, endActivity);
	}

	int FrameMan::SwitchResolution(int newResX, int newResY, int newMultiplier, bool endActivity) {
		if (!IsValidResolution(newResX, newResY) || newResX <= 0 || newResX > m_ScreenRes->w || newResY > m_ScreenRes->h) {
			return -1;
		}

		if (g_ActivityMan.GetActivity()) {
			g_ActivityMan.EndActivity();
		}

		SDL_RenderClear(m_Renderer);

		if (!m_Fullscreen) {
			SDL_SetWindowSize(m_Window, newResX, newResY);
			SDL_GetWindowSize(m_Window, &m_ResX, &m_ResY);

			SDL_RenderSetLogicalSize(m_Renderer, m_ResX, m_ResY);
			SDL_RenderSetScale(m_Renderer, newMultiplier, newMultiplier);

			m_NewResX = m_ResX;
			m_NewResY = m_ResY;
		} else {
			SDL_RenderSetLogicalSize(m_Renderer, m_ResX, m_ResY);

			SDL_RenderSetScale(m_Renderer, newMultiplier, newMultiplier);

			m_NewResX = m_ResX = newResX;
			m_NewResY = m_ResY = newResY;
		}

		m_ResMultiplier = m_NewResMultiplier = newMultiplier;

		g_SettingsMan.UpdateSettingsFile();

		m_ResChanged = true;

		return 0;
	}

	void FrameMan::SetFullscreen(bool fullscreen) {
		if (SDL_SetWindowFullscreen(m_Window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) == 0) {
			m_Fullscreen = fullscreen;
			if (fullscreen)
				SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(m_Window), m_ScreenRes.get());
			else
				SDL_GetDisplayUsableBounds(SDL_GetWindowDisplayIndex(m_Window), m_ScreenRes.get());
		} else {
			g_ConsoleMan.PrintString("ERROR: " + std::string(SDL_GetError()));
		}
	}

	int FrameMan::CalculateTextWidth(const std::string &text, bool isSmall) {
		return isSmall ? GetSmallFont()->CalculateWidth(text) : GetLargeFont()->CalculateWidth(text);
	}

	int FrameMan::CalculateTextHeight(const std::string &text, int maxWidth, bool isSmall) {
		return isSmall ? GetSmallFont()->CalculateHeight(text, maxWidth) : GetLargeFont()->CalculateHeight(text, maxWidth);
	}

	void FrameMan::SetScreenText(std::string message, int whichScreen, int blinkInterval, long displayDuration, bool centered) {
		if (whichScreen >= 0 && whichScreen < c_MaxScreenCount && m_TextDurationTimer[whichScreen].IsPastRealMS(m_TextDuration[whichScreen])) {
			m_ScreenText[whichScreen] = message;
			m_TextDuration[whichScreen] = displayDuration;
			m_TextDurationTimer[whichScreen].Reset();
			m_TextBlinking[whichScreen] = blinkInterval;
			m_TextCentered[whichScreen] = centered;
		}
	}

	void FrameMan::ClearScreenText(int whichScreen) {
		if (whichScreen >= 0 && whichScreen < c_MaxScreenCount) {
			m_ScreenText[whichScreen].clear();
			m_TextDuration[whichScreen] = -1;
			m_TextDurationTimer[whichScreen].Reset();
			m_TextBlinking[whichScreen] = 0;
		}
	}

	GUIFont *FrameMan::GetFont(bool isSmall) {
		if (!m_GUIScreen) {
			m_GUIScreen = std::make_shared<SDLScreen>();
		}

		if (isSmall) {
			if (!m_SmallFont) {
				m_SmallFont = std::make_shared<GUIFont>("SmallFont");
				m_SmallFont->Load(m_GUIScreen.get(), "Base.rte/GUIs/Skins/Base/smallfont.png");
			}
			return m_SmallFont.get();
		}
		if (!m_LargeFont) {
			m_LargeFont = std::make_shared<GUIFont>("FatFont");
			m_LargeFont->Load(m_GUIScreen.get(), "Base.rte/GUIs/Skins/Base/fatfont.png");
		}
		return m_LargeFont.get();
	}
	void FrameMan::DrawScreenText(int playerScreen, SDLGUITexture playerGUIBitmap) {}

	void FrameMan::FlashScreen(int screen, uint32_t color, float periodMS) {
		m_FlashScreenColor[screen] = color;
		m_FlashTimer[screen].SetRealTimeLimitMS(periodMS);
		m_FlashTimer[screen].Reset();
	}

	uint32_t FrameMan::GetColorFromIndex(uint32_t color) const {
		return m_Palette->getPixel(color);
	}

} // namespace RTE
