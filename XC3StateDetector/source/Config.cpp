#include "Config.hpp"

Config::Config(const InitData& init)
	: IScene(init)
{
	if (not getData().ini)
	{
		throw Error{ U"Failed to load `config.ini`" };
	}
	AppLanguage = Parse<String>(getData().ini[U"Language.AppLanguage"]);
	GameLanguage = Parse<String>(getData().ini[U"Language.GameLanguage"]);
	HDMICapture = Parse<String>(getData().ini[U"Device.HDMICapture"]);
	SerialPort = Parse<String>(getData().ini[U"Device.SerialPort"]);
	ScreenshotFolderPath = Parse<String>(getData().ini[U"Screenshot.FolderPath"]);
	ScreenshotFileName = Parse<String>(getData().ini[U"Screenshot.FileName"]);
	ScreenshotDateFormat = Parse<String>(getData().ini[U"Screenshot.DateFormat"]);
	ScreenshotFileFormat = Parse<String>(getData().ini[U"Screenshot.FileFormat"]);
	enableToastNotification = Parse<bool>(getData().ini[U"Notification.enableToastNotification"]);

	for (int i = 0; i < LanguageSelection.size(); ++i)
	{
		OpenableListBoxAppLanguage.emplace_back(Translate[AppLanguage][LanguageSelection[i]]);
		OpenableListBoxGameLanguage.emplace_back(Translate[AppLanguage][LanguageSelection[i]]);
	}

	size_t AppLanguageIndex = 0;
	for (size_t i = 0; i < LanguageSelection.size(); ++i)
	{
		if (LanguageSelection[i] == getData().AppLanguage)
		{
			AppLanguageIndex = i;
			break;
		}
	}
	OpenableListBoxAppLanguage.setIndex(AppLanguageIndex);

	size_t GameLanguageIndex = 0;
	for (size_t i = 0; i < LanguageSelection.size(); ++i)
	{
		if (LanguageSelection[i] == getData().GameLanguage)
		{
			GameLanguageIndex = i;
			break;
		}
	}
	OpenableListBoxGameLanguage.setIndex(GameLanguageIndex);

	for (const auto& info : System::EnumerateWebcams())
	{
		HDMICaptures.push_back(info.name);
	};
	OpenableListBoxHDMICapture.setItems(HDMICaptures);

	for (const auto& info : System::EnumerateSerialPorts())
	{
		OpenableListBoxSerialPort.emplace_back(U"{} {}"_fmt(info.hardwareID, info.description));
	}

	for (int i = 0; i < ScreenshotDateFormatSelection.size(); ++i)
	{
		OpenableListBoxScreenshotDateFormat.emplace_back(ScreenshotDateFormatSelection[i]);
	}
	OpenableListBoxScreenshotDateFormat.setIndexFromItem(getData().ScreenshotDateFormat);

	for (int i = 0; i < ScreenshotFileFormatSelection.size(); ++i)
	{
		OpenableListBoxScreenshotFileFormat.emplace_back(ScreenshotFileFormatSelection[i]);
	}
	OpenableListBoxScreenshotFileFormat.setIndexFromItem(getData().ScreenshotFileFormat);

	ScreenshotFileNameTextEditState.text = getData().ScreenshotFileName;
}

void Config::update()
{
	OpenableListBoxScreenshotFileFormat.update();
	OpenableListBoxScreenshotDateFormat.update();
	OpenableListBoxSerialPort.update();
	OpenableListBoxHDMICapture.update();
	OpenableListBoxGameLanguage.update();
	OpenableListBoxAppLanguage.update();
	AppLanguage = LanguageSelection[OpenableListBoxAppLanguage.getSelectedIndex()];
	GameLanguage = LanguageSelection[OpenableListBoxGameLanguage.getSelectedIndex()];
	HDMICapture = OpenableListBoxHDMICapture.getSelectedItem();
	SerialPort = OpenableListBoxSerialPort.getSelectedItem();
	ScreenshotDateFormat = OpenableListBoxScreenshotDateFormat.getSelectedItem();
	ScreenshotFileFormat = OpenableListBoxScreenshotFileFormat.getSelectedItem();
	if (reload)
	{
		changeScene(U"Config");
	}
}

void Config::draw() const
{
	DrawVerticalGradientBackground(ColorF{ 0.2, 0.5, 1.0 }, ColorF{ 0.5, 0.8, 1.0 });
	FontAsset(U"TitleFont")(U"Config").draw(TitlePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Game Language"]).draw(GameLanguagePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"This Application Language"]).draw(AppLanguagePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"HDMI Capture"]).draw(HDMICapturePos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Serial Port"]).draw(SerialPortPos);
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Screenshot"]).draw(ScreenshotFolderPos);
	String now = FormatDateTime(DateTime::Now(), ScreenshotDateFormat);
	FontAsset(U"TextFont")(U"{} : {}{}{}{}"_fmt(Translate[AppLanguage][U"Example"], ScreenshotFolderPath, ScreenshotFileName, now, ScreenshotFileFormat)).draw(ScreenshotFolderPos.movedBy(0, 100));
	FontAsset(U"SubtitleFont")(Translate[AppLanguage][U"Notification Enable Toast"]).draw(NotificationToastPos);

	SimpleGUI::CheckBox(enableToastNotification, Translate[AppLanguage][U"Toast notification upon completion"], NotificationToastPos);
	OpenableListBoxScreenshotDateFormat.draw();
	OpenableListBoxScreenshotFileFormat.draw();

	if (SimpleGUI::Button(Translate[AppLanguage][U"Change the save folder"], ScreenshotFolderPos.movedBy(0, 50)))
	{
		const auto result = Dialog::SelectFolder(getData().ScreenshotFolderPath);
		if (result)
		{
			ScreenshotFolderPath = result.value();
		}
	}

	if (SimpleGUI::TextBox(ScreenshotFileNameTextEditState, ScreenshotFileNamePos, ScreenshotFileNameTextWidth, ScreenshotFileNameMaxCharacters))
	{
		ScreenshotFileName = ScreenshotFileNameTextEditState.text;
	}

	OpenableListBoxSerialPort.draw();
	OpenableListBoxHDMICapture.draw();
	OpenableListBoxGameLanguage.draw();
	OpenableListBoxAppLanguage.draw();

	if (SimpleGUI::Button(Translate[AppLanguage][U"Apply"], ApplyButtonPos))
	{
		getData().AppLanguage = AppLanguage;
		getData().GameLanguage = GameLanguage;
		getData().HDMICapture = HDMICapture;
		getData().SerialPort = SerialPort;
		getData().ScreenshotFolderPath = ScreenshotFolderPath;
		getData().ScreenshotFileName = ScreenshotFileName;
		getData().ScreenshotDateFormat = ScreenshotDateFormat;
		getData().ScreenshotFileFormat = ScreenshotFileFormat;
		getData().enableToastNotification = enableToastNotification;

		getData().ini[U"Language.AppLanguage"] = AppLanguage;
		getData().ini[U"Language.GameLanguage"] = GameLanguage;
		getData().ini[U"Device.HDMICapture"] = HDMICapture;
		getData().ini[U"Device.SerialPort"] = SerialPort;
		getData().ini[U"Screenshot.FolderPath"] = ScreenshotFolderPath;
		getData().ini[U"Screenshot.FileName"] = ScreenshotFileName;
		getData().ini[U"Screenshot.DateFormat"] = ScreenshotDateFormat;
		getData().ini[U"Screenshot.FileFormat"] = ScreenshotFileFormat;
		getData().ini[U"Notification.enableToastNotification"] = enableToastNotification;
		getData().ini.save(U"config.ini");
		reload = true;
	}
}
