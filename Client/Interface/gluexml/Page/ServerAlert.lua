SERVER_ALERT_PTR_URL = "";
SERVER_ALERT_URL = "http://127.0.0.1/fr/alert";

MainMenu = "Vide"; --"Interface/Glues/Models/UI_MainMenu_BurningCrusade/UI_MainMenu_BurningCrusade.m2";
MainScreen = MainScreen;
LoginMainScreen = nil;
MainLogin = false;
mt_count = 10;

ligne = " ==>"..MainMenu.."\n";
	
anim = false;
nextC, nextCset, blend_start = nextC, nextCset, blend_start;
timed_update, blend_timer, music_timer = 0, 0, 0;
elapsed = 1;
use_random_starting_scene = false	;													-- boolean: false = always starts with sceneID 1   ||   true = starts with a random sceneID
shuffle_scenes_randomly = false;	

current_scene = 1;
MScene = {};
-- or MScene;
ModelList = {} ;
--or ModelList;

-- main frame for displaying and positioning of the whole loginscreen
LoginScene = nil;
Login_on = false;

LoginCam = 0;
width, height = 0,0;

-- main background that changes according to the scene
LoginScreenBackground = nil;
LoginScreenBlackBoarderTOP = nil;
LoginScreenBlackBoarderBOTTOM = nil;
LoginScreenBlend = nil;
LoginGlueAmbience="DARKPORTAL";
LoginGlueAmbienceVol = 4.0;

		
	ModelList = {
		max_scenes = 1,			-- number of scenes you use to shuffle through
		fade_duration = 1,		-- fade animation duration in seconds (to next scene if more than 1 exists)
		modelCount = 0,
		sceneCount = 0,
		lastModelNum = 0,
		sceneData = {}
	};

function ServerAlert_OnLoad(self)
	self:RegisterEvent("SHOW_SERVER_ALERT");

end

function ServerAlert_OnEvent(self, event, ...)
	if ( event == "SHOW_SERVER_ALERT" ) then
		local text = ...;
		--We need to call SetWidth(0) so that GetWidth() doesn't return 0.
		--Clearly.
		--(Not needed in 7.0, see bug 449258)
		HidePage();
		local AlertText = "";
		if (text > "") then
			if (ModelList.modelCount == 0) then
				AlertText = PageLogin(text) or "";
				if (AlertText > "") then
					-- ServerAlertText:SetText(AlertText);
					-- ServerAlertFrame:Show();
				end
				text = AlertText;
			end
		end
		
		if (ModelList.modelCount == 0) then
			if(MainMenu == "Vide") then
				MainMenu = "Interface\\Glues\\Models\\UI_MainMenu_BurningCrusade\\UI_MainMenu_BurningCrusade.m2";
			end
			-- MainMenu = "Interface\\Glues\\Models\\UI_MainMenu_Northrend\\UI_MainMenu_Northrend.m2";
			-- MainMenu = "Interface\\Glues\\Models\\UI_MainMenu_BurningCrusade\\UI_MainMenu_BurningCrusade.m2";
			LoginMainScreen:SetModel(MainMenu );
			LoginMainScreen:SetCamera(0);
			LoginMainScreen:SetSequence(0);
			LoginMainScreen:Show();
			--LoginMainScreen:SetModel("Interface\Glues\Models\UI_MainMenu_BurningCrusade\UI_MainMenu_BurningCrusade.m2");
		else
			ShowPage(1);
			text = "";
		end
		
		if(text > "") then
			self.ScrollFrame:SetWidth(0);
			--We have to resize before calling SetText because SimpleHTML frames won't resize correctly.
			self.ScrollFrame.Text:SetWidth(self.ScrollFrame:GetWidth());
			self.ScrollFrame.Text:SetText(text);
			self.isActive = true;
			if ( not self.disabled ) then
				self:Show();
			end
		end
	end
end

function ServerAlert_Disable(self)
	self:Hide()
	self.disabled = true;
end

function ServerAlert_Enable(self)
	self.disabled = false;
	if ( self.isActive ) then
		self:Show();
	end
end
