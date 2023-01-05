#pragma once

#include <Wiwa/core/Core.h>
#include "../vendor/rapidjson/document.h"

namespace Wiwa {
	class WI_API JSONValue {
	private:
		rapidjson::Value* m_Value;
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>* m_Allocator;
	public:
		JSONValue(rapidjson::Value* value, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>* allocator);
		~JSONValue();

		template<class T> void set(T data);
		template<class T> void operator=(T data);

		// Get value
		template<class T> T get();

		// Get value as
		const char* as_string() { return get<const char*>(); }
		int as_int() { return get<int>(); }
		unsigned int as_uint() { return get<unsigned int>(); }
		float as_float() { return get<float>(); }
		bool as_bool() { return get<bool>(); }
		double as_double() { return get<double>(); }
		int64_t as_int64() { return get<int64_t>(); }
		uint64_t as_uint64() { return get<uint64_t>(); }

		// Cast operator
		template<class T> operator T();

		template<class T>
		JSONValue AddMember(const char* mem, T value);

		JSONValue AddMemberObject(const char* mem);

		bool HasMember(const char* mem);

		JSONValue operator[](const char* key);
	};

	template<class T>
	inline void JSONValue::set(T data)
	{
		m_Value->Set<T>(data);
	}

	template<class T>
	inline T JSONValue::get()
	{
		return m_Value->Get<T>();
	}

	template<>
	inline const char* JSONValue::get()
	{
		return m_Value->GetString();
	}

	template<class T>
	inline void JSONValue::operator=(T data)
	{
		set<T>(data);
	}

	template<class T>
	inline JSONValue::operator T()
	{
		return get<T>();
	}

	template<class T>
	inline JSONValue JSONValue::AddMember(const char* mem, T value)
	{
		rapidjson::Value key(mem, *m_Allocator);
		rapidjson::Value v(value);

		rapidjson::Value& jval = m_Value->AddMember(key, v, *m_Allocator);

		return JSONValue(&jval, m_Allocator);
	}

	template<>
	inline JSONValue JSONValue::AddMember<const char*>(const char* mem, const char* value) {
		rapidjson::Value key;
		key.SetString(mem, *m_Allocator);
		rapidjson::Value v;
		v.SetString(value, *m_Allocator);

		rapidjson::Value& jval = m_Value->AddMember(key, v, *m_Allocator);

		return JSONValue(&jval, m_Allocator);
	}
}