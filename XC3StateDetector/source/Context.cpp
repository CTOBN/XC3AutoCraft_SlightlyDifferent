#include "State.hpp"
#include "Context.hpp"

namespace xc3
{
	const HashTable<String, String> StateNameToDescription = {
		{U"Undefined", U"何もしていません"},
		{U"TitleLoading", U"タイトル画面をローディング中です"},
		{U"Title", U"タイトル画面です"},
		{U"FieldLoading", U"フィールドをローディング中です"},
		{U"Field", U"フィールドです"},
		{U"Camp", U"休憩ポイントです"},
		{U"RecognizeItemCount", U"アンノウンマターの数を認識しています"},
		{U"AccessoryTypeSelecting", U"アクセサリの種類を選択しています"},
		{U"AccessoryTypeSelected", U"アクセサリ選択画面です"},
		{U"Judge", U"アクセサリを判断しています"},
		{U"GotDesiredAcc", U"目的のアクセサリを手に入れました"},
		{U"GoingMainMenu", U"メインメニューを開いています"},
		{U"MainMenu", U"メインメニューです"},
		{U"SystemMenu", U"システムメニューです"},
	};

	Context::Context(Serial& serial)
		: serial(serial)
	{
	}

	void Context::setState(std::unique_ptr<State> newState) {
		m_state = std::move(newState);
	}

	void Context::deleteState() {
		m_state = nullptr;
	}

	void Context::request()
	{
		if (m_state == nullptr)
		{
			return;
		}
		m_state->handle(*this);
	}
	void Context::setSerial(Serial& _serial) {
		this->serial = _serial;
	}

	String Context::getCurrentStateName() const
	{
		if (m_state == nullptr)
		{
			return U"Undefined";
		}
		return m_state->getName();
	}

	String Context::getCurrentStateDescription() const
	{
		if (m_state == nullptr)
		{
			return U"何もしていません";
		}
		if (StateNameToDescription.find(m_state->getName()) != StateNameToDescription.end()) {
			return StateNameToDescription.at(m_state->getName());
		}
		return U"何もしていません";
	}

	bool Context::getCurrentStateHasTransitioned() const
	{
		if (m_state == nullptr)
		{
			return false;
		}
		return m_state->hasTransitioned;
	}

	void Context::init()
	{
		gotDesiredAccessory = false;
		canMakeAccessory = false;
		isJudged = true;
		isUnkownMatterCountUpdated = true;
		isAccessoryTypeSelected = true;
	}
}

