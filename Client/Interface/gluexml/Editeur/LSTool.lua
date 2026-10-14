
--[[## DISABLE LOGIN FUNCTIONALITY FOR EDITING ##]] do
	AccountLogin.UI.AccountEditBox:Hide()
	AccountLogin.UI.PasswordEditBox:Hide()
	AccountLogin.UI.LoginButton:Hide()
	AccountLogin.UI.SaveAccountNameCheckButton:Hide()
	AccountLogin.UI.CreditsButton:Hide()
	AccountLogin.UI.CinematicsButton:Hide()
	AccountLogin.UI.OptionsButton:Hide()
	AccountLogin.UI.CommunityButton:Hide()
	AccountLogin.UI.ManageAccountButton:Hide()
	AccountLogin.UI.CreateAccountButton:Hide()
	AccountLogin.UI.WoWAccountSelectDialog:Hide()
	AccountLogin.UI.CaptchaEntryDialog:Hide()
	AccountLogin.UI.AccountsDropDown:Hide()
	AccountLogin.UI.GameLogo:Hide()
	AccountLogin.UI.Blizz:Hide()
	AccountLogin.UI.ClientVersion:Hide()
	AccountLogin.UI.BlizzardLogo:Hide()
	
	--AccountLogin.UI.SaveAccountNameText:Hide()
	--if AccountLogin.UI.SavePassword then
		--AccountLogin.UI.SavePassword:Hide()
		--AccountLogin.UI.SavePasswordText:Hide()
	--end
	
	function AccountLogin_Login() end
	function AccountLogin_OnShow(self) end
	function AccountLogin_UpdateSavedData(self) end
	
--[[## DISABLE LOGIN FUNCTIONALITY FOR EDITING ##]] end

--[[############### defining variables ###############]] do

	CurrentModelSelected = false
	nM = false
	scrollOffset = 0
	LSmodels = GetModel(current_scene,true)
	mData = GetModelData(current_scene,true)
	currentSaveText = ""
	--cbMax = (GlueParent:GetHeight()/3 - 35) / 26
	cbMax = (LoginScene:GetHeight()/3 - 35) / 26
	LSCButtons = {}
	
--[[##################################################]] end

--[[############### defining backdrops ###############]] do
	backdropColor = DEFAULT_TOOLTIP_COLOR;
	backdropTF = {		-- TOOLS FRAME BACKDROP
		bgFile = "Interface\\DialogFrame\\UI-DialogBox-Background",
		edgeFile = "Interface\\DialogFrame\\UI-DialogBox-Border",
		tile = true,
		tileSize = 32,
		edgeSize = 32,
		insets = {
			left = 11,
			right = 12,
			top = 12,
			bottom = 11
		}
	}
	backdropST = {		-- NEW EDITBOX BACKDROP
		bgFile = "Interface\\Tooltips\\UI-Tooltip-Background",
		edgeFile = "Interface\\Glues\\Common\\Glue-Tooltip-Border",
		tile = true,
		tileSize = 16,
		edgeSize = 16,
		insets = {
			left = 10,
			right = 5,
			top = 4,
			bottom = 9
		}
	}
--[[##################################################]] end

--[[################ defining methods ################]] do
	function round_after(num)
		return string.format("%.3f", num)
	end
	
	function Round_AfterLua(num)
		return string.format("%.3f", num)
	end
	
	function newEditBox()
		--eb = CreateFrame("EditBox",nil,LoginScene)
		eb = CreateFrame("EditBox",nil,AccountLogin.UI)
		--eb:SetSize(GlueParent:GetWidth()/2, 30)
		eb:SetSize(AccountLogin.UI:GetWidth()/2, 30)
		eb:SetPoint("CENTER")
		eb:SetBackdrop(backdropST)
		eb:SetFrameStrata("TOOLTIP")
		eb:SetBackdropBorderColor(backdropColor[1], backdropColor[2], backdropColor[3])
		eb:SetBackdropColor(backdropColor[4], backdropColor[5], backdropColor[6])
		eb:SetFontObject("GlueEditBoxFont")
		eb:SetTextInsets(12,5,5,12)
		eb:Hide()
		
		return eb
	end
	
	function setCodePage(idx)
		local sText=""
		if idx == 2 then
			for i=1,ModelList.sceneCount do
				local texte = getSaveLoginString(i)
				sText = sText.."\n"..texte
			end
		else
			for i=1,ModelList.sceneCount do
				local texte = getSaveString(i)
				sText = sText.."\n"..texte
			end					
		end
		sText = string.sub(sText,1,string.len(sText)-2)
		return sText
	end

	function getSaveString(sceneID)
		local TEXT = "	-- Scene: "..(sceneID).."\n"
		local sceneID, sceneData, models, modeldatas = GetScene(sceneID)
		
		for num,data in pairs(modeldatas) do
			local m = models[num]
			local z,x,y = m:GetPosition()
			local width,height = m:GetSize()
			TEXT = TEXT.."	{"..
				sceneID..", "..
				round_after(x)..", "..
				round_after(y)..", "..
				round_after(z)..", "..
				round_after(m:GetFacing())..", "..
				round_after(m:GetModelScale())..", "..
				round_after(m:GetAlpha())..", "..
				(data[8] or "_")..", "..
				data[9]..", "..
				(LoginScene:GetWidth() / width)..", "..
				(LoginScene:GetHeight() / height)..", "..
				( (data[12] and '"'..data[12]..'"') or "_")..", "..
				(data[13] or "_")..", "..
				( (data[14] and '"'..data[14]..'"') or "_").."},\n"
		end
		
		return TEXT
	end
	
	function getSaveLoginString(sceneID)
		local TEXT = "SERVERALERT:VIDE\nPAGE:\n"
		TEXT = TEXT.."|G["..sceneID..","..login_tempo..","..(M[sceneID].background or "")..","
		TEXT = TEXT ..PosRx..","..PosRy..","..PosLx..","..PosLy.."]\n"
		TEXT = TEXT.."|M["..login_music_path..","..login_music_time_in_seconds.."]\n";
		
		local scene, sceneData, models, modeldatas = GetScene(sceneID)
		local LoginLigne = 0;
		local Memo = "";
		
		for num,data in pairs(modeldatas) do
			local m = models[num];
			local z,x,y = m:GetPosition();
			local width,height = m:GetSize();
			local DataLogin=data;
			
			LoginLigne=LoginLigne+1;
			
			if (data[14]==data[12]) then
				DataLogin[14]="m";
			end
			
			if Memo>"" then
				if DataLogin[12]~=Memo then
					Memo=DataLogin[12];
				else
					DataLogin[12]="m";
				end
			else
				Memo=DataLogin[12];
			end

			TEXT = TEXT.."|H"..tonumber(LoginLigne).."["..
				scene..","..
				Round_AfterLua(x)..","..
				Round_AfterLua(y)..","..
				Round_AfterLua(z)..","..
				Round_AfterLua(m:GetFacing())..","..
				round_after(m:GetModelScale())..","..
				round_after(m:GetAlpha())..","..
				(DataLogin[8] or "")..","..
				DataLogin[9]..","..
				(LoginScene:GetWidth() / width)..","..
				(LoginScene:GetHeight() / height)..","..
				(DataLogin[12] or "")..","..
				(DataLogin[13] or "")..","..
				(DataLogin[14] or "").."]\n"
		end
		TEXT = TEXT.."\n"
		return TEXT
	end
	
	function getSauveFondEcran(sceneID)
		local TEXT = "	-- Scene: "..(sceneID).."\n"
		local sceneID, sceneData, models, modeldatas = GetScene(sceneID)
		
		for num,data in pairs(modeldatas) do
			local m = models[num]
			local z,x,y = m:GetPosition()
			local width,height = m:GetSize()
			TEXT = TEXT.."	{"..
				sceneID..", "..
				round_after(x)..", "..
				round_after(y)..", "..
				round_after(z)..", "..
				round_after(m:GetFacing())..", "..
				round_after(m:GetModelScale())..", "..
				round_after(m:GetAlpha())..", "..
				(data[8] or "_")..", "..
				data[9]..", "..
				(LoginScene:GetWidth() / width)..", "..
				(LoginScene:GetHeight() / height)..", "..
				( (data[12] and '"'..data[12]..'"') or "_")..", "..
				(data[13] or "_")..", "..
				( (data[14] and '"'..data[14]..'"') or "_").."},\n"
		end
		
		return TEXT
	end
	
	function updateStatsText()
		local m = LSmodels[buttonData[CurrentModelSelected][2]]
		local z,x,y = m:GetPosition()
		LSEFText:SetText(""..
			"X: ".. round_after(x) .."\n"..
			"Y: ".. round_after(y) .."\n"..
			"Z: ".. round_after(z) .."\n"..
			"O: ".. round_after(m:GetFacing()) .."\n"..
			"Alpha: ".. round_after(m:GetAlpha()) .."\n"..
			"Scale: ".. round_after(m:GetModelScale()))
	end
	
	function updateScrollFrame(delta)
		if #buttonData > cbMax then
			if delta < 0 then
				if scrollOffset + cbMax <= #buttonData then
					scrollOffset = scrollOffset + 1
					
					for i=1,cbMax do
						if buttonData[i+scrollOffset] then
							LSCButtons[i]:Show()
							_G[LSCButtons[i]:GetName().."Text"]:SetText(buttonData[i+scrollOffset][1])
						else
							LSCButtons[i]:Hide()
						end
					end
					
					LSScrollBarKnob:SetPoint("RIGHT", LSToolsFrame, "LEFT", 10, 60 - scrollParts * scrollOffset)
				end
			else
				if scrollOffset - 1 >= 0 then
					scrollOffset = scrollOffset - 1
					
					for i=1,cbMax do
						if buttonData[i+scrollOffset] then
							LSCButtons[i]:Show()
							_G[LSCButtons[i]:GetName().."Text"]:SetText(buttonData[i+scrollOffset][1])
						else
							LSCButtons[i]:Hide()
						end
					end
					
					LSScrollBarKnob:SetPoint("RIGHT", LSToolsFrame, "LEFT", 10, 60 - scrollParts * scrollOffset)
				end
			end
			for i=1,cbMax do
				if buttonData[i+scrollOffset][3] then
					LSCButtons[i]:SetChecked(1)
				else
					LSCButtons[i]:SetChecked(0)
				end
			end
		end
	end
--[[##################################################]] end

--[[################ defining objects ################]] do

	--[[## creating TOOLFRAME frame ##]] do
		LSToolsFrame = CreateFrame("Frame",nil,LoginScene)
			LSToolsFrame:SetBackdrop(backdropTF)
			LSToolsFrame:SetSize(200, LoginScene:GetHeight()/3)
			LSToolsFrame:SetFrameStrata("HIGH")
			LSToolsFrame:SetPoint("BOTTOM", AccountLogin.UI.OptionsButton, "TOP", -15, 50)
			LSToolsFrame:EnableMouseWheel(true)
			LSToolsFrame:Hide()

		--[[SCROLL]]--
		LSToolsFrame:SetScript("OnMouseWheel", function(self, delta)
				updateScrollFrame(delta)
			end)
	--[[########################]] end

	--[[## creating SCROLLBAR frame ##]] do
		LSScrollBar = CreateFrame("Frame",nil,LSToolsFrame)
		
		--[[SHOW]]--
		LSScrollBar:SetScript("OnShow", function()
				LSScrollBarKnob:SetPoint("RIGHT", LSToolsFrame, "LEFT", 10, 60)
			end)
	--[[############################]] end

	--[[## creating SCROLLUP button ##]] do
		LSScrollUp = CreateFrame("Button",nil,LSScrollBar,"GlueScrollUpButtonTemplate")
			LSScrollUp:SetPoint("TOPRIGHT", LSToolsFrame, "TOPLEFT", 10, -40)
			
		--[[CLICK]]--
		LSScrollUp:SetScript("OnClick", function()
				updateScrollFrame(1)
			end)
	--[[############################]] end

	--[[## creating SCROLLKNOB texture ##]] do
		LSScrollBarKnob = LSScrollBar:CreateTexture(nil,"OVERLAY")
			LSScrollBarKnob:SetTexture("Interface/Buttons/UI-ScrollBar-Knob")
			LSScrollBarKnob:SetSize(18,24)
			LSScrollBarKnob:SetTexCoord(0.20, 0.80, 0.125, 0.875)
	--[[#####################################]] end

	--[[## creating SCROLLDOWN button ##]] do
		LSScrollDown = CreateFrame("Button",nil,LSScrollBar,"GlueScrollDownButtonTemplate")
			LSScrollDown:SetPoint("BOTTOMRIGHT", LSToolsFrame, "BOTTOMLEFT", 10, 10)

		--[[CLICK]]--
		LSScrollDown:SetScript("OnClick", function()
				updateScrollFrame(-1)
			end)
	--[[############################]] end

	--[[## creating TOOL button ##]] do
		LSToolsButton = CreateFrame("Button",nil,LoginScene,"GlueButtonSmallTemplate")
			LSToolsButton:SetText("TOOLBAR")
			LSToolsButton:SetPoint("BOTTOM", AccountLogin.UI.OptionsButton, "TOP", 0, 0)
			LSToolsButton:SetFrameStrata("HIGH")

		--[[CLICK]]--
		LSToolsButton:SetScript("OnClick", function()
				if LSToolsFrame:IsVisible() then LSToolsFrame:Hide() else LSToolsFrame:Show() end
			end)
	--[[##########################]] end
	
	--[[## creating Fond d'ecran button ##]] do
		LSFondEcranButton = CreateFrame("Button",nil,LoginScene,"GlueButtonSmallTemplate")
			LSFondEcranButton:SetText("Fond d'ecran")
			LSFondEcranButton:SetPoint("BOTTOM", LSToolsButton, "BOTTOM",0,-LSToolsButton:GetHeight() - 2)
			LSFondEcranButton:SetFrameStrata("HIGH")

		--[[CLICK]]--
		LSFondEcranButton:SetScript("OnClick", function()
				if LSFondEcranText:IsVisible() then LSFondEcranText:Hide() else LSFondEcranText:Show() end
			end)
	--[[##########################]] end

--[[## creating Fond d'ecran editbox ##]] do
		LSFondEcranText = newEditBox()
			local LSFDText = LSFondEcranText:CreateFontString("LSFDText", "OVERLAY", "GlueEditBoxFont")
			LSFDText:SetPoint("RIGHT", LSFondEcranText, "LEFT", 0, 3)
			LSFDText:SetText("Chemin du fond d'ecran :")

		--[[SHOW]]--
		LSFondEcranText:SetScript("OnShow", function(self)
				self:SetText("")
				LSFondEcranDone:Show()
			end)
			
		--[[ENTERPRESSED]]--
		LSFondEcranText:SetScript("OnEnterPressed", function()
				if LSFondEcranText:GetText()~="" then
					M[current_scene].background = LSFondEcranText:GetText()
					LoginScreenBackground:SetTexture(M[current_scene].background)
				end
				
				LSFondEcranText:Hide()
				LSFondEcranDone:Hide()
			end)
			
		--[[ESCAPEPRESSED]]--
		LSFondEcranText:SetScript("OnEscapePressed", function()
				LSFondEcranText:Hide()
			end)
	--[[##################################]] end
		
	--[[## creating FondEcranDone button ##]] do
		LSFondEcranDone = CreateFrame("Button",nil,AccountLogin,"GlueButtonSmallTemplate")
			LSFondEcranDone:SetText("Done")
			LSFondEcranDone:SetWidth(50)
			LSFondEcranDone:SetPoint("LEFT", LSFondEcranText, "RIGHT", -5, 0)
			LSFondEcranDone:SetFrameStrata("TOOLTIP")
			LSFondEcranDone:Hide()
			
		--[[CLICK]]--
		LSFondEcranDone:SetScript("OnClick", function()
				LSFondEcranDone:Hide()
				LSFondEcranText:Hide()
			end)
	--[[##############################]] end
	
	--[[## creating SAVE button ##]] do
		LSSave = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSSave:SetText("Save")
			LSSave:SetPoint("TOPLEFT", LSToolsFrame, "TOPLEFT", 12, -5)
			LSSave:SetWidth(50)

		--[[CLICK]]--
		LSSave:SetScript("OnClick", function(self, button, down)
				if not down then
					LSSaveText:Show()
					LSSaveLogin:Show()
					LSSaveDone:Show()
					LSSaveFrame:Show()
				end
			end)
	--[[##########################]] end

	--[[## creating SAVETEXT editbox ##]] do
		LSSaveText = newEditBox()
			LSSaveText:SetMultiLine(true)
			
		--[[SHOW]]--
		LSSaveText:SetScript("OnShow", function()
				local sText = ""
				-- |G[1,10,Background.blp,,125,-1,-125]
				
				for i=1,ModelList.sceneCount do
					local TEXT = getSaveString(i)
					sText = sText.."\n"..TEXT
				end
				sText = string.sub(sText,1,string.len(sText)-2)
				LSSaveText:SetText(sText)
				currentSaveText = sText
				LSSaveText:HighlightText()
			end)

		--[[ENTERPRESSED]]--
		LSSaveText:SetScript("OnEnterPressed", function()
				LSSaveDone:Hide()
				LSSaveLogin:Hide()
				LSSaveText:Hide()
				LSSaveFrame:Hide()
			end)

		--[[CHAR]]--
		LSSaveText:SetScript("OnChar", function()
				LSSaveText:SetText(currentSaveText)
				LSSaveText:HighlightText()
			end)

		--[[MOUSEUP]]--
		LSSaveText:SetScript("OnMouseUp", function()
				LSSaveText:HighlightText()
			end)
	--[[###########################]] end

	--[[## creating SAVEDONE button ##]] do
		LSSaveDone = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSSaveDone:SetText("Done")
			LSSaveDone:SetWidth(50)
			LSSaveDone:SetPoint("LEFT", LSSaveText, "RIGHT", -5, 0)
			LSSaveDone:SetFrameStrata("TOOLTIP")
			LSSaveDone:Hide()
			
		--[[CLICK]]--
		LSSaveDone:SetScript("OnClick", function()
				LSSaveDone:Hide()
				LSSaveLogin:Hide()
				LSSaveText:Hide()
				LSSaveFrame:Hide()
			end)
	--[[##############################]] end	
	
	--[[## creating SAVELOGIN button ##]] do
		LSSaveLogin = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSSaveLogin:SetText("Login")
			LSSaveLogin:SetWidth(50)
			LSSaveLogin:SetPoint("LEFT", LSSaveText, "RIGHT", -5, 25)
			LSSaveLogin:SetFrameStrata("TOOLTIP")
			LSSaveLogin:Hide()
			
		--[[CLICK]]--
		LSSaveLogin:SetScript("OnClick", function()
				local sText=""
				if LSSaveLogin:GetText()~="LUA" then
					LSSaveLogin:SetText("LUA")
					sText = setCodePage(2)
				else
					LSSaveLogin:SetText("Login")

					sText = setCodePage(1)
				end
				
				sText = string.sub(sText,1,string.len(sText)-2)
				LSSaveText:SetText(sText)
				currentSaveText = sText
				LSSaveText:HighlightText()
			end)
			
		LSSaveLogin:SetScript("OnShow", function()
				local sText=""
				if LSSaveLogin:GetText()=="LUA" then
					sText = setCodePage(2)
				else
					sText = setCodePage(1)					
				end
				sText = string.sub(sText,1,string.len(sText)-2)
				LSSaveText:SetText(sText)
				currentSaveText = sText
				LSSaveText:HighlightText()
			end)
			
	--[[##############################]] end
	
	-- --[[## creating SAVEDONE button ##]] do
		-- LSSaveDone = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			-- LSSaveDone:SetText("Done")
			-- LSSaveDone:SetWidth(50)
			-- LSSaveDone:SetPoint("LEFT", LSSaveText, "RIGHT", -5, 0)
			-- LSSaveDone:SetFrameStrata("TOOLTIP")
			-- LSSaveDone:Hide()
			
		-- --[[CLICK]]--
		-- LSSaveDone:SetScript("OnClick", function()
				-- LSSaveDone:Hide()
				-- LSSaveText:Hide()
				-- LSSaveFrame:Hide()
			-- end)
	-- --[[##############################]] end
	
	--[[## creating SAVEFRAME frame ##]] do
		-- LSToolsFrame:SetSize(200, AccountLogin:GetHeight()/3)
		-- LSSaveFrame = CreateFrame("Frame",nil,GlueParent)
		LSSaveFrame = CreateFrame("Frame",nil,LoginScene)
			LSSaveFrame:SetFrameStrata("DIALOG")
			-- LSSaveFrame:SetAllPoints(GlueParent)
			LSSaveFrame:SetAllPoints(AccountLogin.UI.OptionsButton)
			LSSaveFrame:EnableMouse(true)
			LSSaveFrame:EnableKeyboard(true)
			LSSaveFrame:Hide()
	--[[##############################]] end
	
	--[[## creating SAVEBACKGROUND texture ##]] do
		LSSaveBackground = LSSaveFrame:CreateTexture(nil,"OVERLAY")
			LSSaveBackground:SetAllPoints(AccountLogin.UI.OptionsButton)
			LSSaveBackground:SetTexture(0,0,0,0.75)
	--[[#####################################]] end

	--[[## creating NEW button ##]] do
		LSNew = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSNew:SetText("New")
			LSNew:SetPoint("LEFT", LSSave, "RIGHT", -2, 0)
			LSNew:SetWidth(40)

		--[[CLICK]]--
		LSNew:SetScript("OnClick", function()
				LSNewCameraText:Show()
				LSNewDone:Show()
				LSNewCancel:Show()
				LSSaveFrame:Show()
			end)

		--[[UPDATE]]--
		LSNew:SetScript("OnUpdate", function()
				if CurrentModelSelected then
					LSDelete:Show()
					LSEditingFrame:Show()
					updateStatsText()
				else
					LSDelete:Hide()
					LSEditingFrame:Hide()
				end
			end)
	--[[#########################]] end

	--[[## creating NEWCAMERATEXT editbox ##]] do
		LSNewCameraText = newEditBox()
			local LSNCTText = LSNewCameraText:CreateFontString("LSNCTText", "OVERLAY", "GlueEditBoxFont")
			LSNCTText:SetPoint("RIGHT", LSNewCameraText, "LEFT", 0, 3)
			LSNCTText:SetText("(Optional)Camera Path:")

		--[[SHOW]]--
		LSNewCameraText:SetScript("OnShow", function(self)
				self:SetText("")
			end)
			
		--[[ENTERPRESSED]]--
		LSNewCameraText:SetScript("OnEnterPressed", function()
				if LSNewCameraText:GetText()~="" then
					nM = newModel(M[current_scene].parent,1,_,1,1,LSNewCameraText:GetText())
				else
					nM = newModel(M[current_scene].parent,1,_,1,1)
				end
				nM:Hide()
				
				LSNewText:Hide()
				LSNewCameraText:Hide()
				LSNewText:Show()
			end)
			
		--[[ESCAPEPRESSED]]--
		LSNewCameraText:SetScript("OnEscapePressed", function()
				LSNewCameraText:Hide()
				LSNewDone:Hide()
				LSNewCancel:Hide()
				LSSaveFrame:Hide()
			end)
	--[[##################################]] end
	
	--[[## creating NEWTEXT editbox ##]] do
		LSNewText = newEditBox()
			local LSNTText = LSNewText:CreateFontString("LSNTText", "OVERLAY", "GlueEditBoxFont")
			LSNTText:SetPoint("RIGHT", LSNewText, "LEFT", 0, 3)
			LSNTText:SetText("Model Path:")

		--[[SHOW]]--
		LSNewText:SetScript("OnShow", function(self)
				self:SetText("")
			end)

		--[[ENTERPRESSED]]--
		LSNewText:SetScript("OnEnterPressed", function()
				if LSNewText:GetText() ~= "" then
					if nM:GetModel() == "character/human/male/humanmale.m2" then
						ModelList[ModelList.modelCount+1] = {current_scene,0,0,0,0,1,1,_,1,1,1,LSNewText:GetText(),_,_}
					else
						ModelList[ModelList.modelCount+1] = {current_scene,0,0,0,0,1,1,_,1,1,1,LSNewText:GetText(),_,nM:GetModel()}
					end
					nM:SetModel(LSNewText:GetText())
					nM:Show()
					M[current_scene][ModelList.modelCount+1] = nM
					nM = nil
					
					ModelList.modelCount = ModelList.modelCount + 1
					
					LSNewText:Hide()
					LSNewDone:Hide()
					LSNewCancel:Hide()
					LSSaveFrame:Hide()
					
					Scene_OnStart(current_scene, true)
					CurrentModelSelected = false
				end
			end)
			
		--[[ESCAPEPRESSED]]--
		LSNewText:SetScript("OnEscapePressed", function()
				LSNewText:Hide()
				LSNewDone:Hide()
				LSNewCancel:Hide()
				LSSaveFrame:Hide()
			end)
	--[[##############################]] end

	--[[## creating NEWDONE button ##]] do
		LSNewDone = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSNewDone:SetText("Done")
			LSNewDone:SetWidth(50)
			LSNewDone:SetFrameStrata("TOOLTIP")
			LSNewDone:SetPoint("TOP", LSNewCameraText, "BOTTOM", -65, 0)
			LSNewDone:Hide()
			
		--[[CLICK]]--
		LSNewDone:SetScript("OnClick", function()
				if LSNewCameraText:IsShown() then
					if LSNewCameraText:GetText()~="" then
						nM = newModel(M[current_scene].parent,1,_,1,1,LSNewCameraText:GetText())
					else
						nM = newModel(M[current_scene].parent,1,_,1,1)
					end
					nM:Hide()
					
					LSNewText:Hide()
					LSNewCameraText:Hide()
					LSNewText:Show()
				elseif LSNewText:IsShown() then
					if LSNewText:GetText() ~= "" then
						if nM:GetModel() == "character/human/male/humanmale.m2" then
							ModelList[ModelList.modelCount+1] = {current_scene,0,0,0,0,1,1,_,1,1,1,LSNewText:GetText(),_,_}
						else
							ModelList[ModelList.modelCount+1] = {current_scene,0,0,0,0,1,1,_,1,1,1,LSNewText:GetText(),_,nM:GetModel()}
						end
						nM:SetModel(LSNewText:GetText())
						nM:Show()
						M[current_scene][ModelList.modelCount+1] = nM
						nM = nil
						
						ModelList.modelCount = ModelList.modelCount + 1
						
						LSNewText:Hide()
						LSNewDone:Hide()
						LSNewCancel:Hide()
						LSSaveFrame:Hide()
						
						Scene_OnStart(current_scene, true)
						CurrentModelSelected = false
					end
				end
			end)
	--[[#############################]] end

	--[[## creating NEWCANCEL button ##]] do
		LSNewCancel = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSNewCancel:SetText("Cancel")
			LSNewCancel:SetWidth(70)
			LSNewCancel:SetFrameStrata("TOOLTIP")
			LSNewCancel:SetPoint("LEFT", LSNewDone, "RIGHT", 5, 0)
			LSNewCancel:Hide()
			
		--[[CLICK]]--
		LSNewCancel:SetScript("OnClick", function()
				LSNewCameraText:Hide()
				LSNewText:Hide()
				LSNewDone:Hide()
				LSNewCancel:Hide()
				LSSaveFrame:Hide()
			end)
	--[[#############################]] end

	--[[## creating DELETE button ##]] do
		LSDelete = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplate")
			LSDelete:SetText("Delete")
			LSDelete:SetPoint("LEFT", LSNew, "RIGHT", -2, 0)
			LSDelete:SetWidth(60)
			
		--[[CLICK]]--
		LSDelete:SetScript("OnClick", function()
				if CurrentModelSelected then
					local num = buttonData[CurrentModelSelected][2]
					LSmodels[num]:SetAlpha(0)
					ModelList[num] = nil
					
					for i=1,cbMax do
						LSCButtons[i]:SetChecked(0)
					end
					
					Scene_OnStart(current_scene, true)
					CurrentModelSelected = false
				end
			end)
	--[[############################]] end

	--[[## creating SCENES button ##]] do
		LSScenes = CreateFrame("Button",nil,LSToolsFrame,"GlueButtonSmallTemplateBlue")
			LSScenes:SetText("NS")
			LSScenes:SetPoint("TOPRIGHT", LSToolsFrame, "TOPRIGHT", -8, -5)
			LSScenes:SetWidth(35)

		--[[CLICK]]--
		LSScenes:SetScript("OnClick", function()
				if ModelList.sceneCount > 1 then
					CurrentModelSelected = false
				end
				local newScene = ((current_scene+1 > ModelList.max_scenes) and 1) or current_scene + 1
				SetScene(newScene)
				LSScenes:SetText(newScene)
			end)

		--[[SHOW]]--
		LSScenes:SetScript("OnShow", function()
				LSScenes:SetText(current_scene)
			end)

		--[[UPDATE]]--
		LSScenes:SetScript("OnUpdate", function()
				blend_timer = 0
				LoginScreenBlend:SetAlpha(0)
			end)
	--[[############################]] end

	--[[## creating EDITFRAME frame ##]] do
		LSEditingFrame = CreateFrame("Frame",nil,LoginScene)
			LSEditingFrame:SetBackdrop(backdropTF)
			LSEditingFrame:SetHeight(130)
			LSEditingFrame:SetFrameStrata("HIGH")
			LSEditingFrame:SetPoint("BOTTOMRIGHT", LSToolsFrame, "TOPRIGHT", 0, -7)
			LSEditingFrame:SetPoint("LEFT", LSToolsFrame, "LEFT", 0, 0)
			LSEditingFrame:EnableMouse(true)
			LSEditingFrame:Hide()
			
		--[[ENTER]]--
		LSEditingFrame:SetScript("OnEnter", function()
			LSEFControls:Show()
		end)
		
		--[[LEAVE]]--
		LSEditingFrame:SetScript("OnLeave", function()
			LSEFControls:Hide()
		end)
		
		--[[UPDATE]]--
		LSEditingFrame:SetScript("OnUpdate", function()
			overwriteFunction_MoveModel()
		end)
	--[[########################]] end
	
	--[[## creating INTENSITY slider ##]] do
		LSIntensity = CreateFrame("Button",nil,LSEditingFrame,"GlueButtonSmallTemplate")
			LSIntensity:SetText("100%")
			LSIntensity:SetPoint("TOP", LSEditingFrame, "TOP", 0, -25)
			LSIntensity:SetSize(40,15)
			LSIntensity:RegisterForDrag("LeftButton")
			LSIntensity:SetID(100)
			local LSILine = LSIntensity:CreateTexture(nil,"BACKGROUND")
				LSILine:SetTexture(0,0,0,0.75)
				LSILine:SetHeight(2)
				LSILine:SetPoint("TOP", LSIntensity, "TOP", 0, -5)
				LSILine:SetPoint("LEFT", LSEditingFrame, "LEFT", 20, 0)
				LSILine:SetPoint("RIGHT", LSEditingFrame, "RIGHT", -20, 0)
			local LSIText = LSIntensity:CreateFontString("LSIText", "OVERLAY", "GlueFontNormalSmall")
				LSIText:SetPoint("BOTTOM", LSIntensity, "TOP", 0, 5)
				LSIText:SetPoint("LEFT", LSEditingFrame, "LEFT", 12, 0)
				LSIText:SetText("Modify Intensity:")
			
		local Ix,Iy = LSIntensity:GetCenter()
		local draging = false
		--[[DOUBLECLICK]]--
		LSIntensity:SetScript("OnDoubleClick", function()
				LSIntensity:SetPoint("TOP", LSEditingFrame, "TOP", 0, -25)
				LSIntensity:SetText("100%")
				LSIntensity:SetID(100)
			end)
			
		--[[MOUSEDOWN]]--
		LSIntensity:SetScript("OnMouseDown", function()
				draging = true
			end)
			
		--[[MOUSEUP]]--
		LSIntensity:SetScript("OnMouseUp", function()
				draging = false
			end)
			
		--[[DRAGSTOP]]--
		LSIntensity:SetScript("OnDragStop", function()
				draging = false
			end)
			
		--[[UPDATE]]--
		LSIntensity:SetScript("OnUpdate", function()
				if draging then
					local mx,my = GetCursorPosition()
					if mx - Ix < 70 and mx - Ix > -69 then
						local number = math.floor((100/70) * (mx - Ix) + 100)
						LSIntensity:SetPoint("TOP", LSEditingFrame, "TOP", mx - Ix, -25)
						LSIntensity:SetText(number.."%")
						LSIntensity:SetID(number)
					elseif mx - Ix < 70 then
						LSIntensity:SetPoint("TOP", LSEditingFrame, "TOP", -70, -25)
						LSIntensity:SetText("1%")
						LSIntensity:SetID(1)
					elseif mx - Ix > -70 then
						LSIntensity:SetPoint("TOP", LSEditingFrame, "TOP", 70, -25)
						LSIntensity:SetText("200%")
						LSIntensity:SetID(200)
					end
				end
			end)
	--[[############################]] end

	--[[## creating STATS text ##]] do
		LSEFText = LSEditingFrame:CreateFontString("LSEFText", "OVERLAY", "GlueFontNormalSmall")
			LSEFText:SetPoint("TOP", LSEditingFrame, "TOP", 0, -42)
			LSEFText:SetPoint("LEFT", LSEditingFrame, "LEFT", 12, 0)
			LSEFText:SetPoint("RIGHT", LSEditingFrame, "CENTER", 0, 0)
			LSEFText:SetPoint("BOTTOM", LSEditingFrame, "BOTTOM", 0, 10)
			LSEFText:SetJustifyH("LEFT")
			LSEFText:SetJustifyV("TOP")
	--[[############################]] end

	--[[## creating ANIMATIONDOWN button ##]] do
		LSAnimationDown = CreateFrame("Button",nil,LSEditingFrame,"GlueScrollDownButtonTemplate")
			LSAnimationDown:SetPoint("BOTTOMRIGHT", LSEditingFrame, "BOTTOMRIGHT", -10, 10)

		--[[CLICK]]--
		LSAnimationDown:SetScript("OnClick", function()
				if mData[buttonData[CurrentModelSelected][2]][9] > 1 then
					mData[buttonData[CurrentModelSelected][2]][9] = mData[buttonData[CurrentModelSelected][2]][9] - 1
				end
				LSAnimation:SetText(mData[buttonData[CurrentModelSelected][2]][9])
				LSmodels[buttonData[CurrentModelSelected][2]]:SetSequence(mData[buttonData[CurrentModelSelected][2]][9])
			end)
	--[[############################]] end

	--[[## creating ANIMATION text ##]] do
		LSAnimation = LSEditingFrame:CreateFontString("LSEFText", "OVERLAY", "GlueFontNormalSmall")
			LSAnimation:SetPoint("BOTTOM", LSAnimationDown, "TOP", 0, 2)
			LSAnimation:SetText(1)
	--[[############################]] end

	--[[## creating ANIMATIONUP button ##]] do
		LSAnimationUp = CreateFrame("Button",nil,LSEditingFrame,"GlueScrollUpButtonTemplate")
			LSAnimationUp:SetPoint("BOTTOM", LSAnimation, "TOP", 0, 1)

		--[[SHOW]]--
		LSAnimationUp:SetScript("OnShow", function()
				LSAnimation:SetText(mData[buttonData[CurrentModelSelected][2]][9])
			end)

		--[[CLICK]]--
		LSAnimationUp:SetScript("OnClick", function()
				mData[buttonData[CurrentModelSelected][2]][9] = mData[buttonData[CurrentModelSelected][2]][9] + 1
				LSAnimation:SetText(mData[buttonData[CurrentModelSelected][2]][9])
				LSmodels[buttonData[CurrentModelSelected][2]]:SetSequence(mData[buttonData[CurrentModelSelected][2]][9])
			end)
	--[[############################]] end

	--[[## creating CONTROLS text ##]] do
		LSEFControls = LSEditingFrame:CreateFontString("LSEFText", "OVERLAY", "GlueFontNormalSmall")
			LSEFControls:SetPoint("BOTTOMRIGHT", LSEditingFrame, "BOTTOMLEFT", 0, 15)
			LSEFControls:SetJustifyH("RIGHT")
			LSEFControls:SetJustifyV("BOTTOM")
			LSEFControls:Hide()
		
		LSEFControls:SetText(""..
			"Hold left Shift / Ctrl / Alt for mouse controls\n\n"..
			"left / right  -->  A / D\n"..
			"up / down  -->  W / S\n"..
			"further / nearer  -->  X / C\n"..
			"turn left / right  -->  Q / E\n"..
			"alpha more / few  -->  T / G\n"..
			"bigger / smaller  -->  R / F\n")
	--[[############################]] end

--[[##################################################]] end

--[[############### defining overwrite ###############]] do
	AccountLogin:EnableMouse(true)

	AccountLogin:SetScript("OnKeyDown", function (self, key)
		if CurrentModelSelected and not LSNewDone:IsShown() then
			local m = LSmodels[buttonData[CurrentModelSelected][2]]
			local x,y,z = m:GetPosition()
			local move = 0.1 * LSIntensity:GetID()/100
			if key=="LSHIFT" then
				SHIFT_MODIFIER = true
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n\n\n"..
					"left-mousebutton --> X&Y\n"..
					"right-mousebutton --> O\n\n\n")
			elseif key=="LCTRL" or key=="STRG" then
				CTRL_MODIFIER = true
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n\n\n"..
					"left-mousebutton --> Scale\n"..
					"right-mousebutton --> Z\n\n\n")
			elseif key=="LALT" then
				ALT_MODIFIER = true
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n\n\n"..
					"left-mousebutton --> X\n"..
					"right-mousebutton --> Y\n\n\n")
			elseif key=="W" then
				m:SetPosition(x,y,z + move)
			elseif key=="S" then
				m:SetPosition(x,y,z - move)
			elseif key=="A" then
				m:SetPosition(x,y - move,z)
			elseif key=="D" then
				m:SetPosition(x,y + move,z)
			elseif key=="Q" then
				local o = m:GetFacing()
				o = (o + move) % (math.pi*2)
				m:SetFacing(o)
			elseif key=="E" then
				local o = m:GetFacing()
				o = o - move
				if o < 0 then
					o = math.pi*2 + o
				end
				m:SetFacing(o)
			elseif key=="R" then
				m:SetModelScale(m:GetModelScale() + move)
			elseif key=="F" then
				m:SetModelScale(m:GetModelScale() - move)
			elseif key=="X" then
				m:SetPosition(x - move,y,z)
			elseif key=="C" then
				m:SetPosition(x + move,y,z)
			elseif key=="T" then
				m:SetAlpha(m:GetAlpha() + move)
			elseif key=="G" then
				m:SetAlpha(m:GetAlpha() - move)
			end
		end
	end)
	
	AccountLogin:SetScript("OnKeyUp", function(self, key)
			if key=="LSHIFT" then
				SHIFT_MODIFIER = false
				mouse_Editing_Models = false
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n"..
					"left / right  -->  A / D\n"..
					"up / down  -->  W / S\n"..
					"further / nearer  -->  X / C\n"..
					"turn left / right  -->  Q / E\n"..
					"alpha more / few  -->  T / G\n"..
					"bigger / smaller  -->  R / F\n")
			elseif key=="LCTRL" or key=="STRG" then
				CTRL_MODIFIER = false
				mouse_Editing_Models = false
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n"..
					"left / right  -->  A / D\n"..
					"up / down  -->  W / S\n"..
					"further / nearer  -->  X / C\n"..
					"turn left / right  -->  Q / E\n"..
					"alpha more / few  -->  T / G\n"..
					"bigger / smaller  -->  R / F\n")
			elseif key=="LALT" then
				ALT_MODIFIER = false
				mouse_Editing_Models = false
				LSEFControls:SetText(""..
					"Hold left Shift / Ctrl / Alt for mouse controls\n\n"..
					"left / right  -->  A / D\n"..
					"up / down  -->  W / S\n"..
					"further / nearer  -->  X / C\n"..
					"turn left / right  -->  Q / E\n"..
					"alpha more / few  -->  T / G\n"..
					"bigger / smaller  -->  R / F\n")
			end
		end)
	
	AccountLogin:SetScript("OnMouseDown", function(self, button)
			if SHIFT_MODIFIER then
				local m = LSmodels[buttonData[CurrentModelSelected][2]]
				if button=="LeftButton" then
					mouse_Editing_Models = "XY"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_X, sModel_Y, sModel_Z = m:GetPosition()
				elseif button=="RightButton" then
					mouse_Editing_Models = "O"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_O = m:GetFacing()
				end
			elseif CTRL_MODIFIER then
				local m = LSmodels[buttonData[CurrentModelSelected][2]]
				if button=="LeftButton" then
					mouse_Editing_Models = "Scale"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_Scale = m:GetModelScale()
				elseif button=="RightButton" then
					mouse_Editing_Models = "Z"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_X, sModel_Y, sModel_Z = m:GetPosition()
				end
			elseif ALT_MODIFIER then
				local m = LSmodels[buttonData[CurrentModelSelected][2]]
				if button=="LeftButton" then
					mouse_Editing_Models = "X"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_X, sModel_Y, sModel_Z = m:GetPosition()
				elseif button=="RightButton" then
					mouse_Editing_Models = "Y"
					sMouse_X, sMouse_Y = GetCursorPosition()
					sModel_X, sModel_Y, sModel_Z = m:GetPosition()
				end
			end
		end)
	
	AccountLogin:SetScript("OnMouseUp", function(self, button)
			if button=="LeftButton" then
				mouse_Editing_Models = false
			elseif button=="RightButton" then
				mouse_Editing_Models = false
			end
		end)
		
	function overwriteFunction_MoveModel()
		if mouse_Editing_Models then
			local move = 0.003 * LSIntensity:GetID()/100
			local m = LSmodels[buttonData[CurrentModelSelected][2]]
			local mx,my = GetCursorPosition()
			if mouse_Editing_Models == "XY" then
				m:SetPosition( sModel_X, sModel_Y + ( (mx - sMouse_X) * move ), sModel_Z + ( (my - sMouse_Y) * move ) )
			elseif mouse_Editing_Models == "O" then
				local nFac = sModel_O + ( (mx - sMouse_X) * move )
				if nFac > math.pi*2 then
					nFac = nFac - math.pi*2
				elseif nFac < 0 then
					nFac = math.pi*2 + nFac
				end
				m:SetFacing( nFac )
			elseif mouse_Editing_Models == "Scale" then
				m:SetModelScale( sModel_Scale + ( (my - sMouse_Y) * move ) )
			elseif mouse_Editing_Models == "Z" then
				m:SetPosition( sModel_X + ( (my - sMouse_Y) * move ), sModel_Y, sModel_Z)
			elseif mouse_Editing_Models == "X" then
				m:SetPosition( sModel_X, sModel_Y + ( (mx - sMouse_X) * move ), sModel_Z)
			elseif mouse_Editing_Models == "Y" then
				m:SetPosition( sModel_X, sModel_Y, sModel_Z + ( (my - sMouse_Y) * move ))
			end
		end
	end
	
	function SceneUpdate() end
	function Scene_OnEnd() end
	function Scene_OnStart(sceneID, maybeCheck)
		if LSLastScene ~= sceneID or maybeCheck then
			buttonData = {}
			LSmodels = GetModel(sceneID,true)
			mData = GetModelData(sceneID,true)
			scrollOffset = 0
			if mData then
				for num,m in pairs(mData) do
					local str = m[12]
					local startingSTR = strlen(str)-18
					local startingDOT = ".."
					if startingSTR < 1 then
						startingSTR = 1
						startingDOT = ""
					end
					str = startingDOT..strsub(str, startingSTR, strlen(str))
					table.insert(buttonData, { str , num, false } )
				end
			end
			
			for i=1,cbMax do
				if buttonData[i] then
					LSCButtons[i]:Show()
					_G[LSCButtons[i]:GetName().."Text"]:SetText(buttonData[i][1])
					LSCButtons[i]:SetChecked(0)
				else
					LSCButtons[i]:Hide()
				end
			end
			
			if #buttonData > cbMax then
				LSScrollBar:Show()
				scrollParts = 150 / (#buttonData - cbMax + 0.5)
				LSScrollBarKnob:SetPoint("RIGHT", LSToolsFrame, "LEFT", 10, 60)
			else
				LSScrollBar:Hide()
			end
			
			LSLastScene = sceneID
		end
	end
	
	for i=1,cbMax do	-- BUTTON ADDING LOOP
		local b = CreateFrame("CheckButton", "LSModelCheckButton"..i, LSToolsFrame, "GlueCheckButtonTemplate")
			_G[b:GetName().."Text"]:SetText("")
			_G[b:GetName().."Text"]:SetPoint("LEFT",b,"LEFT",17,8)
			_G[b:GetName().."Text"]:SetDrawLayer("OVERLAY")
			b:SetNormalTexture("Interface/Glues/Common/Glue-Panel-Button-Up")
			b:SetPushedTexture("Interface/Glues/Common/Glue-Panel-Button-Down")
			b:SetHighlightTexture("Interface/Glues/Common/Glue-Panel-Button-Highlight")
			b:SetCheckedTexture("Interface/Glues/Common/Glue-Panel-Button-Down-Blue")
			b:SetWidth(360)
			b:SetHeight(45)
			b:SetPoint("CENTER")
			b:SetFrameStrata("HIGH")
			b:SetHitRectInsets(0,b:GetWidth()/3,-5,b:GetHeight()/3)
			if i < 2 then
				b:SetPoint("TOPLEFT", LSToolsFrame, "TOPLEFT", 0,-35)
			else
				b:SetPoint("TOPLEFT", LSCButtons[i-1], "TOPLEFT", 0,-26)
			end
		
		b:SetScript("OnLeave", function()
				for num,m in pairs(LSmodels) do
					m:SetAlpha(mData[num][7])
				end
			end)
		
		b:SetScript("OnEnter", function()
				for num,m in pairs(LSmodels) do
					mData[num][7] = m:GetAlpha()
					if buttonData[i+scrollOffset][2] == num then
						m:SetAlpha(1)
					else
						m:SetAlpha(0.2)
					end
				end
			end)
		
		b:SetScript("OnClick", function()
				if b:GetChecked() then
					for j=1,cbMax do
						if i~=j then
							LSCButtons[j]:SetChecked(0)
						end
					end
					for j=1,#buttonData do
						buttonData[j][3] = false
					end
					CurrentModelSelected = i+scrollOffset
					buttonData[i+scrollOffset][3] = true
				else
					CurrentModelSelected = false
					buttonData[i+scrollOffset][3] = false
				end
			end)
		
		LSCButtons[i] = b
	end
--[[##################################################]] end


















