#pragma once

struct BaseModel {
    virtual std::string toJson() const = 0;
    virtual std::string getName() const = 0;
};
