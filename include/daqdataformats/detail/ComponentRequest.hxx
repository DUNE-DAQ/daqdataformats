
namespace dunedaq::daqdataformats {


static_assert(std::is_trivially_copyable<ComponentRequest>::value, "ComponentRequest isn't trivially copyable and can't be safely std::memcpy'd");
  
static_assert(std::is_standard_layout<ComponentRequest>::value, "ComponentRequest isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

static_assert(ComponentRequest::s_component_request_version == 2, "This is intentionally designed to tell the developer to update the static_assert checks (including this one) when the version is bumped");
  
static_assert(sizeof(ComponentRequest) == 32, "ComponentRequest struct size different than expected!");
static_assert(offsetof(ComponentRequest, version) == 0, "ComponentRequest version field not at expected offset");
static_assert(offsetof(ComponentRequest, unused) == 4, "ComponentRequest unused field not at expected offset");
static_assert(offsetof(ComponentRequest, component) == 8, "ComponentRequest component field not at expected offset");
static_assert(offsetof(ComponentRequest, window_begin) == 16,
              "ComponentRequest window_begin field not at expected offset");
static_assert(offsetof(ComponentRequest, window_end) == 24, "ComponentRequest window_end field not at expected offset");

  inline std::ostream&
  operator<<(std::ostream& o, ComponentRequest const& cr)
  {
    return o << cr.component << ", begin: " << cr.window_begin << ", end: " << cr.window_end;
  }

} // namespace dunedaq::daqdataformats
