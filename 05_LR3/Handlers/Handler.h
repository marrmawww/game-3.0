#pragma once
#include <memory>
#include <string>

class GameFacade;
class Handler {
public:
    virtual ~Handler() {}
    void setNext(const std::shared_ptr<Handler>& next) { next_ = next; }
    virtual bool handle(const std::string& input, GameFacade& facade);
protected:
    std::shared_ptr<Handler> next_;
};

#define DECLARE_HANDLER(Name) class Name : public Handler { public: bool handle(const std::string& input, GameFacade& facade) override; };
DECLARE_HANDLER(CreateHandler)
DECLARE_HANDLER(MoveHandler)
DECLARE_HANDLER(AttackHandler)
DECLARE_HANDLER(CollectHandler)
DECLARE_HANDLER(HealHandler)
DECLARE_HANDLER(UseHandler)
DECLARE_HANDLER(BuildHandler)
DECLARE_HANDLER(UpgradeHandler)
DECLARE_HANDLER(StatsHandler)
DECLARE_HANDLER(MapHandler)
DECLARE_HANDLER(TurnHandler)
DECLARE_HANDLER(HelpHandler)
#undef DECLARE_HANDLER
