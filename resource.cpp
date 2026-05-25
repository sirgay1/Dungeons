#include "resource.h"

const Iron& Iron::instance() {
    static const Iron inst;
    return inst;
}
const Gold& Gold::instance() {
    static const Gold inst;
    return inst;
}
const Gems& Gems::instance() {
    static const Gems inst;
    return inst;
}
const Exp& Exp::instance() {
    static const Exp inst;
    return inst;
}

const Resource& getResourceByName(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "iron") return Iron::instance();
    if (lower == "gold") return Gold::instance();
    if (lower == "gems") return Gems::instance();
    if (lower == "exp")  return Exp::instance();
    throw std::runtime_error("Unknown resource: " + name);
}