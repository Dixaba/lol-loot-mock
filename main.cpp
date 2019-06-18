#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

#include "include/httplib.h"
#include "include/sqlite_modern_cpp.h"
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/writer.h"

using namespace httplib;

std::string dump_headers(const Headers &headers)
{
  std::stringstream s;

  for (const auto &x : headers)
    {
      s << x.first << ": " << x.second << '\n';
    }

  return s.str();
}

std::string log(const Request &req, const Response &res)
{
  std::stringstream s;
  s << "================================\n";
  s << req.method << ' ' << req.version << ' ' << req.path;
  std::string query;

  for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
      const auto &x = *it;
      s << (it == req.params.begin() ? '?' : '&') << x.first << '=' << x.second;
    }

  s << '\n';
  s << dump_headers(req.headers);
  s << "--------------------------------\n";
  s << res.status << ' ' << res.version;
  s << dump_headers(res.headers);
  s << "\n";

  if (!res.body.empty())
    {
      s << res.body;
    }

  s << "\n";
  return s.str();
}

void fillDB(sqlite::database &db)
{
  db <<
     "CREATE TABLE IF NOT EXISTS loot("
     "asset                     VARCHAR(50)"
     ",count                    INTEGER"
     ",disenchantLootName       VARCHAR(30)"
     ",disenchantValue          INTEGER"
     ",displayCategories        VARCHAR(30)"
     ",expiryTime               INTEGER"
     ",isNew                    BOOLEAN"
     ",isRental                 BOOLEAN"
     ",itemDesc                 VARCHAR(50)"
     ",itemStatus               VARCHAR(30)"
     ",localizedDescription     VARCHAR(1000)"
     ",localizedName            VARCHAR(50)"
     ",localizedRecipeSubtitle  VARCHAR(500)"
     ",localizedRecipeTitle     VARCHAR(50)"
     ",lootId                   VARCHAR(50)  PRIMARY KEY"
     ",lootName                 VARCHAR(50)"
     ",parentItemStatus         VARCHAR(30)"
     ",parentStoreItemId        INTEGER"
     ",rarity                   VARCHAR(50)"
     ",redeemableStatus         VARCHAR(50)"
     ",refId                    INTEGER"
     ",rentalGames              INTEGER"
     ",rentalSeconds            INTEGER"
     ",shadowPath               VARCHAR(50)"
     ",splashPath               VARCHAR(150)"
     ",storeItemId              INTEGER"
     ",tags                     VARCHAR(75)"
     ",tilePath                 VARCHAR(150)"
     ",type                     VARCHAR(30)"
     ",upgradeEssenceName       VARCHAR(30)"
     ",upgradeEssenceValue      INTEGER"
     ",upgradeLootName          VARCHAR(30)"
     ",value                    INTEGER"
     ");";
  FILE *fp = fopen("loot.json", "r"); // non-Windows use "r"

  if (fp != nullptr)
    {
      char readBuffer[65536];
      rapidjson::FileReadStream is(fp, static_cast<char *>(readBuffer),
                                   sizeof(readBuffer));
      rapidjson::Document d;
      d.ParseStream(is);
      fclose(fp);

      if (!d.IsArray())
        { return ;}

      auto ins = db <<
                 "INSERT INTO loot VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";

      try
        {
          for (auto &v : d.GetArray())
            {
              ins << v["asset"].GetString();
              ins << v["count"].GetInt();
              ins << v["disenchantLootName"].GetString();
              ins << v["disenchantValue"].GetInt();
              ins << v["displayCategories"].GetString();
              ins << v["expiryTime"].GetInt();
              ins << v["isNew"].GetBool();
              ins << v["isRental"].GetBool();
              ins << v["itemDesc"].GetString();
              ins << v["itemStatus"].GetString();
              ins << v["localizedDescription"].GetString();
              ins << v["localizedName"].GetString();
              ins << v["localizedRecipeSubtitle"].GetString();
              ins << v["localizedRecipeTitle"].GetString();
              ins << v["lootId"].GetString();
              ins << v["lootName"].GetString();
              ins << v["parentItemStatus"].GetString();
              ins << v["parentStoreItemId"].GetInt();
              ins << v["rarity"].GetString();
              ins << v["redeemableStatus"].GetString();
              ins << v["refId"].GetString();
              ins << v["rentalGames"].GetInt();
              ins << v["rentalSeconds"].GetInt();
              ins << v["shadowPath"].GetString();
              ins << v["splashPath"].GetString();
              ins << v["storeItemId"].GetInt();
              ins << v["tags"].GetString();
              ins << v["tilePath"].GetString();
              ins << v["type"].GetString();
              ins << v["upgradeEssenceName"].GetString();
              ins << v["upgradeEssenceValue"].GetInt();
              ins << v["upgradeLootName"].GetString();
              ins << v["value"].GetInt();
              ins++;
            }
          ins.used(true);
        }
      catch (std::exception &e)
        {
          std::cout << e.what() << std::endl;
        }
    }
  else
    {
      return;
    }
}

std::string getLoot(sqlite::database &db)
{
  using namespace rapidjson;
  Document doc(kArrayType);
  auto &allocator = doc.GetAllocator();
  db << "SELECT * FROM loot;"
     >> [&](std::string asset, int count, std::string disenchantLootName,
            int disenchantValue, std::string displayCategories, int expiryTime, bool isNew,
            bool isRental, std::string itemDesc, std::string itemStatus,
            std::string localizedDescription, std::string localizedName,
            std::string localizedRecipeSubtitle, std::string localizedRecipeTitle,
            std::string lootId, std::string lootName, std::string parentItemStatus,
            int parentStoreItemId, std::string rarity, std::string redeemableStatus,
            std::string refId, int rentalGames, int rentalSeconds, std::string shadowPath,
            std::string splashPath, int storeItemId, std::string tags, std::string tilePath,
            std::string type, std::string upgradeEssenceName, int upgradeEssenceValue,
            std::string upgradeLootName, int value)
  {
    Value lootItem(kObjectType);
    Value val;
    val.SetString(asset.c_str(), allocator);
    lootItem.AddMember("asset", val, allocator);
    val = count;
    lootItem.AddMember("count", val, allocator);
    val.SetString(disenchantLootName.c_str(), allocator);
    lootItem.AddMember("disenchantLootName", val, allocator);
    val = disenchantValue;
    lootItem.AddMember("disenchantValue", val, allocator);
    val.SetString(displayCategories.c_str(), allocator);
    lootItem.AddMember("displayCategories", val, allocator);
    val = expiryTime;
    lootItem.AddMember("expiryTime", expiryTime, allocator);
    val = isNew;
    lootItem.AddMember("isNew", val, allocator);
    val = isRental;
    lootItem.AddMember("isRental", val, allocator);
    val.SetString(itemDesc.c_str(), allocator);
    lootItem.AddMember("itemDesc", val, allocator);
    val.SetString(itemStatus.c_str(), allocator);
    lootItem.AddMember("itemStatus", val, allocator);
    val.SetString(localizedDescription.c_str(), allocator);
    lootItem.AddMember("localizedDescription", val, allocator);
    val.SetString(localizedName.c_str(), allocator);
    lootItem.AddMember("localizedName", val, allocator);
    val.SetString(localizedRecipeSubtitle.c_str(), allocator);
    lootItem.AddMember("localizedRecipeSubtitle", val, allocator);
    val.SetString(localizedRecipeTitle.c_str(), allocator);
    lootItem.AddMember("localizedRecipeTitle", val, allocator);
    val.SetString(lootId.c_str(), allocator);
    lootItem.AddMember("lootId", val, allocator);
    val.SetString(lootName.c_str(), allocator);
    lootItem.AddMember("lootName", val, allocator);
    val.SetString(parentItemStatus.c_str(), allocator);
    lootItem.AddMember("parentItemStatus", val, allocator);
    val = parentStoreItemId;
    lootItem.AddMember("parentStoreItemId", val, allocator);
    val.SetString(rarity.c_str(), allocator);
    lootItem.AddMember("rarity", val, allocator);
    val.SetString(redeemableStatus.c_str(), allocator);
    lootItem.AddMember("redeemableStatus", val, allocator);
    val.SetString(refId.c_str(), allocator);
    lootItem.AddMember("refId", val, allocator);
    val = rentalGames;
    lootItem.AddMember("rentalGames", val, allocator);
    val = rentalSeconds;
    lootItem.AddMember("rentalSeconds", val, allocator);
    val.SetString(shadowPath.c_str(), allocator);
    lootItem.AddMember("shadowPath", val, allocator);
    val.SetString(splashPath.c_str(), allocator);
    lootItem.AddMember("splashPath", val, allocator);
    val = storeItemId;
    lootItem.AddMember("storeItemId", val, allocator);
    val.SetString(tags.c_str(), allocator);
    lootItem.AddMember("tags", val, allocator);
    val.SetString(tilePath.c_str(), allocator);
    lootItem.AddMember("tilePath", val, allocator);
    val.SetString(type.c_str(), allocator);
    lootItem.AddMember("type", val, allocator);
    val.SetString(upgradeEssenceName.c_str(), allocator);
    lootItem.AddMember("upgradeEssenceName", val, allocator);
    val = upgradeEssenceValue;
    lootItem.AddMember("upgradeEssenceValue", val, allocator);
    val.SetString(upgradeLootName.c_str(), allocator);
    lootItem.AddMember("upgradeLootName", val, allocator);
    val = value;
    lootItem.AddMember("value", val, allocator);
    doc.PushBack(lootItem, allocator);
  };
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
}

int main()
{
  sqlite::database db("loot.db");
  fillDB(db);
  Server svr;

  if (!svr.is_valid())
    {
      std::cout << "server has an error..." << std::endl;
      return -1;
    }

  svr.Get("/", [ = ](const Request & /*req*/, Response & res)
  {
    res.set_redirect("/hi");
  });
  svr.Get("/hi", [](const Request & /*req*/, Response & res)
  {
    res.set_content("Hello World!\n", "text/plain");
  });
  svr.Get("/lol-loot/v1/player-loot", [&](const Request & /*req*/, Response & res)
  {
    res.set_content(getLoot(db), "application/json");
  });
  svr.Get("/slow", [](const Request & /*req*/, Response & res)
  {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Slow...\n", "text/plain");
  });
  svr.Get("/dump", [](const Request & req, Response & res)
  {
    res.set_content(dump_headers(req.headers), "text/plain");
  });
  svr.Get("/stop",
  [&](const Request & /*req*/, Response & /*res*/) { svr.stop(); });
  svr.set_error_handler([](const Request & /*req*/, Response & res)
  {
    std::stringstream s;
    s << "<p>Error Status: <span style='color:red;'>" << res.status <<
      "</span></p>";
    res.set_content(s.str(), "text/html");
  });
  svr.set_logger([](const Request & req, const Response & res)
  {
    std::cout << log(req, res) << std::endl;
  });
  svr.listen("localhost", 8080);
  return 0;
}
