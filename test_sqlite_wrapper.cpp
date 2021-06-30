#include "pch.h"

#include <atlstr.h>

#include <memory>
#include <optional>
#include <vector>

#include "../sqlite_manager/sqlite_wrapper.h"

TEST(SqliteTest, CorrectPath8) {
	std::unique_ptr<sqlite_manager::utf8::SqliteWrapper> temp = sqlite_manager::utf8::SqliteWrapper::Create(":memory:");
	ASSERT_NE(temp.get(), nullptr);
}

TEST(SqliteTest, CorrectPath16) {
	std::unique_ptr<sqlite_manager::utf16::SqliteWrapper> temp = sqlite_manager::utf16::SqliteWrapper::Create(L":memory:");
	ASSERT_NE(temp.get(), nullptr);
}

TEST(SqliteTest, WrongPath8) {
	std::wstring utf16 = CA2W("C:\\Users\\ABO\\Desktop\\새 폴더");  // multibyte to utf-16
	std::string utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	std::unique_ptr<sqlite_manager::utf8::SqliteWrapper> temp = sqlite_manager::utf8::SqliteWrapper::Create(utf8);
	ASSERT_EQ(temp.get(), nullptr);
}

TEST(SqliteTest, WrongPath16) {
	std::unique_ptr<sqlite_manager::utf16::SqliteWrapper> temp = sqlite_manager::utf16::SqliteWrapper::Create(L"C:\\Users\\ABO\\Desktop\\새 폴더");
	ASSERT_EQ(temp.get(), nullptr);
}

TEST(SqliteTest, ExecuteUpdate8) {
	std::unique_ptr<sqlite_manager::utf8::SqliteWrapper> temp = sqlite_manager::utf8::SqliteWrapper::Create(":memory:");

	int result = temp->ExecuteUpdate("CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT NOT NULL, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	std::wstring utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트', '123');");  // multibyte to utf-16
	std::string utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트1', '123');");
	utf8 = CW2A(utf16.c_str(), CP_UTF8);

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트2', '123');");
	utf8 = CW2A(utf16.c_str(), CP_UTF8);

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트3', '123');");
	utf8 = CW2A(utf16.c_str(), CP_UTF8);

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	result = temp->ExecuteUpdate("UPDATE items SET hash = '456' WHERE parent_id = 0;");
	ASSERT_EQ(4, result);

	result = temp->ExecuteUpdate("UPDATE items SET hash = '456' WHERE parent_id = 1;");
	ASSERT_EQ(0, result);
}

TEST(SqliteTest, ExecuteUpdate16) {
	std::unique_ptr<sqlite_manager::utf16::SqliteWrapper> temp = sqlite_manager::utf16::SqliteWrapper::Create(L":memory:");
	
	int result = temp->ExecuteUpdate(L"CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT NOT NULL, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트', '123');");
	ASSERT_EQ(1, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트1', '123');");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트2', '123');");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(0, '테스트3', '123');");

	result = temp->ExecuteUpdate(L"UPDATE items SET hash = '456' WHERE parent_id = 0;");
	ASSERT_EQ(4, result);

	result = temp->ExecuteUpdate(L"UPDATE items SET hash = '456' WHERE parent_id = 1;");
	ASSERT_EQ(0, result);
}

TEST(SqliteTest, ExecuteQuery8) {
	std::unique_ptr<sqlite_manager::utf8::SqliteWrapper> temp = sqlite_manager::utf8::SqliteWrapper::Create(":memory:");

	int result = temp->ExecuteUpdate("CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	std::wstring utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(1, '테스트1', NULL);");  // multibyte to utf-16
	std::string utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(2, '테스트2', '123');");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(3, '테스트3', NULL);");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(4, '테스트4', '123');");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	std::optional<std::vector<sqlite_manager::utf8::DataSet>> opt = temp->ExecuteQuery("SELECT * from items;");

	ASSERT_TRUE(opt.has_value());

	std::vector<sqlite_manager::utf8::DataSet> v;
	for (int i = 1; i < 5; ++i) {
		sqlite_manager::utf8::DataSet data;
		std::string id = std::to_string(i);
		data.insert(std::make_pair("id", id));
		data.insert(std::make_pair("parent_id", id));
		data.insert(std::make_pair("name", CW2A(L"테스트", CP_UTF8).m_psz + id));
		if (i % 2) {
			data.insert(std::make_pair("hash", ""));
		}
		else {
			data.insert(std::make_pair("hash", "123"));
		}

		v.push_back(data);
	}

	auto v2 = opt.value();

	for (int i = 0; i < v2.size(); ++i) {
		ASSERT_EQ(v[i], v2[i]);
	}
}

TEST(SqliteTest, ExecuteQuery16) {
	std::unique_ptr<sqlite_manager::utf16::SqliteWrapper> temp = sqlite_manager::utf16::SqliteWrapper::Create(L":memory:");

	int result = temp->ExecuteUpdate(L"CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(1, '테스트1', NULL);");
	ASSERT_EQ(1, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(2, '테스트2', '123');");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(3, '테스트3', NULL);");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(4, '테스트4', '123');");

	std::optional<std::vector<sqlite_manager::utf16::DataSet>> opt = temp->ExecuteQuery(L"SELECT * from items;");

	ASSERT_TRUE(opt.has_value());

	std::vector<sqlite_manager::utf16::DataSet> v;
	for (int i = 1; i < 5; ++i) {
		sqlite_manager::utf16::DataSet data;
		std::wstring id = std::to_wstring(i);
		data.insert(std::make_pair(L"id", id));
		data.insert(std::make_pair(L"parent_id", id));
		data.insert(std::make_pair(L"name", L"테스트" + id));
		if (i % 2) {
			data.insert(std::make_pair(L"hash", L""));
		}
		else {
			data.insert(std::make_pair(L"hash", L"123"));
		}

		v.push_back(data);
	}

	auto v2 = opt.value();

	for (int i = 0; i < v2.size(); ++i) {
		ASSERT_EQ(v[i], v2[i]);
	}
}

TEST(SqliteTest, EmptyQuery8) {
	std::unique_ptr<sqlite_manager::utf8::SqliteWrapper> temp = sqlite_manager::utf8::SqliteWrapper::Create(":memory:");

	int result = temp->ExecuteUpdate("CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	std::wstring utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(1, '테스트1', NULL);");  // multibyte to utf-16
	std::string utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(2, '테스트2', '123');");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(3, '테스트3', NULL);");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	utf16 = CA2W("INSERT INTO items(parent_id, name, hash) VALUES(4, '테스트4', '123');");  // multibyte to utf-16
	utf8 = CW2A(utf16.c_str(), CP_UTF8);  // utf-16 to utf-8

	result = temp->ExecuteUpdate(utf8);
	ASSERT_EQ(1, result);

	std::optional<std::vector<sqlite_manager::utf8::DataSet>> opt = temp->ExecuteQuery("SELECT * from items WHERE parent_id=0;");

	ASSERT_TRUE(opt.has_value());

	auto v = opt.value();
	ASSERT_TRUE(v.empty());
}

TEST(SqliteTest, EmptyQuery16) {
	std::unique_ptr<sqlite_manager::utf16::SqliteWrapper> temp = sqlite_manager::utf16::SqliteWrapper::Create(L":memory:");

	int result = temp->ExecuteUpdate(L"CREATE TABLE IF NOT EXISTS items(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, parent_id INTEGER NOT NULL, name TEXT NOT NULL, hash TEXT, UNIQUE(parent_id, name));");
	ASSERT_EQ(0, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(1, '테스트1', NULL);");
	ASSERT_EQ(1, result);

	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(2, '테스트2', '123');");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(3, '테스트3', NULL);");
	result = temp->ExecuteUpdate(L"INSERT INTO items(parent_id, name, hash) VALUES(4, '테스트4', '123');");

	std::optional<std::vector<sqlite_manager::utf16::DataSet>> opt = temp->ExecuteQuery(L"SELECT * from items WHERE parent_id=0;");

	ASSERT_TRUE(opt.has_value());

	auto v = opt.value();
	ASSERT_TRUE(v.empty());
}