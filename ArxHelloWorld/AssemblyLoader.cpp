#include "StdAfx.h"


class AssemblyLoader {
public:

	static void LoadDllsInCurrentAssemblyDirectory()
	{
		System::AppDomain^ currentDomain = System::AppDomain::CurrentDomain;
		System::Reflection::Assembly^ executingAssembly = System::Reflection::Assembly::GetExecutingAssembly();
		System::String^ assemblyPath = executingAssembly->CodeBase->Replace("file:///", "");
		System::String^ assemblyDirectory = System::IO::Path::GetDirectoryName(assemblyPath);
		array <System::String^>^ dlls = System::IO::Directory::GetFiles(assemblyDirectory, "*.dll");
		array<System::Reflection::Assembly^ >^ assembliesBefore = currentDomain->GetAssemblies();
		for each (System::String ^ dll in dlls) {
			System::Reflection::Assembly^ assembly = GetAssemblyInArrayFromCodeBase(currentDomain->GetAssemblies(), dll);
			if (assembly == nullptr) {
				array<unsigned char>^ assemblyBytes = System::IO::File::ReadAllBytes(dll);
				System::Reflection::Assembly^ assembly = currentDomain->Load(assemblyBytes);
			}
		}
		array<System::Reflection::Assembly^ >^ assembliesAfter = currentDomain->GetAssemblies();
		currentDomain->AssemblyResolve += gcnew System::ResolveEventHandler(AssemblyResolver);
	}

	static System::Reflection::Assembly^ GetAssemblyInArrayFromCodeBase(array<System::Reflection::Assembly^ >^ assemblies, System::String^ codeBase) {
		int max = assemblies->Length;
		for (int i = 0; i < max - 1; i++) {
			System::Reflection::Assembly^ assembly = assemblies[i];
			if (!assembly->IsDynamic) {
				System::String^ codeBasePath = assembly->Location;
				if (codeBasePath == nullptr) {
					codeBasePath = assembly->CodeBase->Replace("file:///", "");
					codeBasePath = codeBasePath->Replace("/", "\\");
				}
				else if (codeBasePath->Empty) {
					codeBasePath = assembly->CodeBase->Replace("file:///", "");
					codeBasePath = codeBasePath->Replace("/", "\\");
				}

				if (codeBasePath != nullptr) {
					codeBasePath = codeBasePath->ToLower();
					if (assembly->CodeBase->Equals(codeBase)) {
						return assembly;
					}
				}
			}
		}
		return nullptr;
	}

	static System::Reflection::Assembly^ GetAssemblyInArray(array<System::Reflection::Assembly^ >^ assemblies, System::String^ name) {
		int max = assemblies->Length;
		for (int i = 0; i < max - 1; i++) {
			System::Reflection::Assembly^ assembly = assemblies[i];
			if (assembly->FullName->Equals(name)) {
				return assembly;
			}
		}
		return nullptr;
	}

	static System::Reflection::Assembly^ AssemblyResolver(System::Object^ sender, System::ResolveEventArgs^ args) {
		System::AppDomain^ currentDomain = System::AppDomain::CurrentDomain;
		array<System::Reflection::Assembly^ >^ assemblies = currentDomain->GetAssemblies();
		System::Reflection::Assembly^ assembly = GetAssemblyInArray(assemblies, args->Name);
		return assembly;
	}
};